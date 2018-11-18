///////////////////////////////////////////////////////////////////////////////
// Sound //////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
/*
 * Copyright (c) 2012 Arduino LLC. All right reserved.
 * Audio library for Arduino Due.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#ifndef SOUND_INCLUDED
#define SOUND_INCLUDED

#include "Print.h"

///////////////////////////////////////////////////////////////////////////////

typedef void (*OnTransmitEnd_CB)(void *data);

///////////////////////////////////////////////////////////////////////////////
// DAC ////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class CDAC
{
public:
  CDAC(Dacc *_dac, uint32_t _dacId, IRQn_Type _isrId) : m_dac(_dac), m_dacId(_dacId), m_isrId(_isrId), m_cb(NULL) { m_cbData = NULL; };

  void begin(uint32_t period) {
    // Enable clock for DAC
    pmc_enable_periph_clk(m_dacId);

    dacc_reset(m_dac);

    // Set transfer mode to double word
    dacc_set_transfer_mode(m_dac, 1);

    // Power save:
    // sleep mode  - 0 (disabled)
    // fast wakeup - 0 (disabled)
    dacc_set_power_save(m_dac, 0, 0);

    // DAC refresh/startup timings:
    // refresh        - 0x08 (1024*8 dacc clocks)
    // max speed mode -    0 (disabled)
    // startup time   - 0x10 (1024 dacc clocks)
    dacc_set_timing(m_dac, 0x08, 0, DACC_MR_STARTUP_1024);

    // Flexible channel selection with tags
    dacc_enable_flexible_selection(m_dac);
    //dacc_set_channel_selection(m_dac, 0);

    // Set up analog current
    dacc_set_analog_control(m_dac,
        DACC_ACR_IBCTLCH0(0x02) |
        DACC_ACR_IBCTLCH1(0x02) |
        DACC_ACR_IBCTLDACCORE(0x01));

    // Enable output channels
    dacc_enable_channel(m_dac, 0);
    dacc_enable_channel(m_dac, 1);

    // Configure Timer Counter to trigger DAC
    // --------------------------------------
    pmc_enable_periph_clk(ID_TC1);
    TC_Configure(TC0, 1,
      TC_CMR_TCCLKS_TIMER_CLOCK2 |  // Clock at MCR/8
      TC_CMR_WAVE |                 // Waveform mode
      TC_CMR_WAVSEL_UP_RC |         // Counter running up and reset when equals to RC
      TC_CMR_ACPA_SET | TC_CMR_ACPC_CLEAR);
    const uint32_t TC = period / 8;
    TC_SetRA(TC0, 1, TC / 2);
    TC_SetRC(TC0, 1, TC);
    TC_Start(TC0, 1);

    // Configure clock source for DAC (2 = TC0 Output Chan. 1)
    dacc_set_trigger(m_dac, 2);

    // Configure pins
    PIO_Configure(g_APinDescription[DAC0].pPort,
        g_APinDescription[DAC0].ulPinType,
        g_APinDescription[DAC0].ulPin,
        g_APinDescription[DAC0].ulPinConfiguration);
    PIO_Configure(g_APinDescription[DAC1].pPort,
        g_APinDescription[DAC1].ulPinType,
        g_APinDescription[DAC1].ulPin,
        g_APinDescription[DAC1].ulPinConfiguration);

    // Enable interrupt controller for DAC
    dacc_disable_interrupt(m_dac, 0xFFFFFFFF);
    NVIC_DisableIRQ(m_isrId);
    NVIC_ClearPendingIRQ(m_isrId);
    NVIC_SetPriority(m_isrId, 0);
    NVIC_EnableIRQ(m_isrId);
  }

  void end() {
    TC_Stop(TC0, 1);
    NVIC_DisableIRQ(m_isrId);
    dacc_disable_channel(m_dac, 0);
    dacc_disable_channel(m_dac, 1);
  }

  bool canQueue() {
    return (m_dac->DACC_TNCR == 0);
  }

  size_t queueBuffer(const uint32_t *buffer, size_t size) {
    // Try the first PDC buffer
    if ((m_dac->DACC_TCR == 0) && (m_dac->DACC_TNCR == 0)) {
      m_dac->DACC_TPR = (uint32_t) buffer;
      m_dac->DACC_TCR = size;
      m_dac->DACC_PTCR = DACC_PTCR_TXTEN;
      if (m_cb)
        dacc_enable_interrupt(m_dac, DACC_IER_ENDTX);
      return size;
    }

    // Try the second PDC buffer
    if (m_dac->DACC_TNCR == 0) {
      m_dac->DACC_TNPR = (uint32_t) buffer;
      m_dac->DACC_TNCR = size;
      m_dac->DACC_PTCR = DACC_PTCR_TXTEN;
      if (m_cb)
        dacc_enable_interrupt(m_dac, DACC_IER_ENDTX);
      return size;
    }

    // PDC buffers full, try again later...
    return 0;
  }

  void setOnTransmitEnd_CB(OnTransmitEnd_CB _cb, void *_data) {
    m_cb = _cb;
    m_cbData = _data;
    if (!m_cb)
      dacc_disable_interrupt(m_dac, DACC_IDR_ENDTX);
  }

  void onService() {
    uint32_t sr = m_dac->DACC_ISR;
    if (sr & DACC_ISR_ENDTX) {
      // There is a free slot, enqueue data
      dacc_disable_interrupt(m_dac, DACC_IDR_ENDTX);
      if (m_cb)
        m_cb(m_cbData);
    }
  }

  void enableInterrupts()  { NVIC_EnableIRQ(m_isrId);  };

  void disableInterrupts() { NVIC_DisableIRQ(m_isrId); };

private:
  Dacc             *m_dac;
  uint32_t          m_dacId;
  IRQn_Type         m_isrId;
  OnTransmitEnd_CB  m_cb;
  void             *m_cbData;
};

///////////////////////////////////////////////////////////////////////////////

CDAC g_DAC(DACC_INTERFACE, DACC_INTERFACE_ID, DACC_ISR_ID);

void DACC_ISR_HANDLER(void) { g_DAC.onService(); }

///////////////////////////////////////////////////////////////////////////////
// ~DAC ///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

class CSound : public Print {
public:
           CSound(CDAC &_dac) : m_dac(&_dac) { bufferSize = 0; buffer = half = last = running = next = NULL; };
  virtual ~CSound(void) {};

  void     begin  (uint32_t sampleRate, uint32_t msPreBuffer);

  void     end    ();

  size_t   write(      uint8_t   c                ) { return 0; /* not implemented */ };
  size_t   write(const uint16_t  *data, size_t size) { return write(reinterpret_cast<const uint32_t*>(data), size/2) * 2; };
  size_t   write(const uint32_t *data, size_t size);

  uint32_t  bufferSize;
  uint32_t *buffer;
  uint32_t *half;
  uint32_t *last;
  uint32_t *volatile running;
  uint32_t *volatile next;
  void        enqueue();

private:
  static void onTransmitEnd(void *me);
  //void        enqueue();
  uint32_t   *cook(const uint32_t *buffer, size_t size);

  CDAC       *m_dac;
};

///////////////////////////////////////////////////////////////////////////////

void CSound::onTransmitEnd(void *_me) {
  CSound *me = reinterpret_cast<CSound *> (_me);
  if (me->running == me->buffer)
    me->running = me->half;
  else
    me->running = me->buffer;
}

///////////////////////////////////////////////////////////////////////////////

void CSound::begin(uint32_t sampleRate, uint32_t msPreBuffer) {
  // Allocate a buffer to keep msPreBuffer milliseconds of audio
  bufferSize = 128;
//  if (bufferSize < 1024)
//    bufferSize = 1024;
  buffer = (uint32_t *) malloc(bufferSize * sizeof(uint32_t));
  half = buffer + bufferSize / 2;
  last = buffer + bufferSize;

  // Buffering starts from the beginning
  running = buffer;
  next = buffer;

  // Start DAC
  m_dac->begin(VARIANT_MCK / sampleRate);

    m_dac->setOnTransmitEnd_CB(onTransmitEnd,this);
}

///////////////////////////////////////////////////////////////////////////////

void CSound::end() {
  m_dac->end();
  free( buffer);
}

///////////////////////////////////////////////////////////////////////////////

//size_t CSound::write(const uint32_t *data, size_t size) {
  //const uint32_t TAG = 0x10000000;
  //Serial.println(size);
//  size_t i;
//  for (i = 0; i < size; i++) {
//    *next = data[i]; //| TAG;
//    next++;}
//  enqueue();
//
//    if (next == half || next == last) {
//      enqueue();
//      while (next == running)
//        ;
//    }
//  }
  //return i;
//}

///////////////////////////////////////////////////////////////////////////////

void CSound::enqueue() {
  if (!m_dac->canQueue()) {
    // DMA queue full
    return;
  }

  if (next == half) {
    // Enqueue the first half
    m_dac->queueBuffer(buffer, bufferSize / 2);
  } else {
    // Enqueue the second half
    m_dac->queueBuffer(half, bufferSize / 2);
    next = buffer; // wrap around
  }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

CSound g_Sound(g_DAC);

#endif

///////////////////////////////////////////////////////////////////////////////
// ~Sound /////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
