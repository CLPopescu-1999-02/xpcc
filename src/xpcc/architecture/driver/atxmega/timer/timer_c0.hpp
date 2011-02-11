// coding: utf-8
// ----------------------------------------------------------------------------
/* Copyright (c) 2011, Roboterclub Aachen e.V.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Roboterclub Aachen e.V. nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ROBOTERCLUB AACHEN E.V. ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL ROBOTERCLUB AACHEN E.V. BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
// ----------------------------------------------------------------------------
/*
 * WARNING: This file is generated automatically, do not edit!
 * Please modify the corresponding *.in file instead and rebuild this file.
 */
// ----------------------------------------------------------------------------


#ifndef XPCC__XMEGA_TIMER_C0_HPP
#define XPCC__XMEGA_TIMER_C0_HPP

#include <avr/io.h>
#include <stdint.h>
#include "hires_c.hpp"
#ifdef AWAXC
#include "awex_c.hpp"
#endif // AWAXC

#if defined(TCC0) || defined(__DOXYGEN__)

namespace xpcc
{
	/**
	 * \brief		Timer Module C0
	 *
	 * \ingroup		atxmega_timer
	 */
#ifndef AWAXC
	class TimerC0 : public HighResC
#else
	class TimerC0 : public HighResC, public WaveformC
#endif // AWAXC
	{
	public:
		inline static TC0_t
		&getModuleBase()
		{
			return TCC0;
		}
		
		inline static void
		setClockSource(TC_CLKSEL_t selection=TC_CLKSEL_DIV1024_gc)
		{
			TCC0_CTRLA = selection;
		}

		inline static void
		setWaveformGenerationMode(TC_WGMODE_t mode=TC_WGMODE_NORMAL_gc)
		{
			TCC0_CTRLB = (TCC0_CTRLB & TC0_WGMODE_gm) | mode;
		}

		inline static void
		setCCEnable(uint8_t selection)
		{
			TCC0_CTRLB = (TCC0_CTRLB & 0x0f) | selection;
		}

		inline static void
		setCompareOutputValue(uint8_t selection)
		{
			TCC0_CTRLC = selection;
		}

		inline static void
		setEventSource(TC_EVSEL_t selection=TC_EVSEL_OFF_gc)
		{
			TCC0_CTRLD = (TCC0_CTRLD & ~TC0_EVSEL_gm) | selection;
		}
		
		inline static void
		setDelayEvent(bool enable=true)
		{
			TCC0_CTRLD = (TCC0_CTRLD & ~TC0_EVDLY_bm) | (enable?TC0_EVDLY_bm:0);
		}
		
		inline static void
		setEventAction(TC_EVACT_t action=TC_EVACT_OFF_gc)
		{
			TCC0_CTRLD = (TCC0_CTRLD & ~TC0_EVSEL_gm) | action;
		}
		
		inline static void
		setByteMode(bool enable=true)
		{
			TCC0_CTRLE = (TCC0_CTRLE & ~TC0_BYTEM_bm) | (enable?TC0_BYTEM_bm:0);
		}
		
		inline static void
		setTimerInterruptLevel(uint8_t level)
		{
			TCC0_INTCTRLA = level;
		}
		
		inline static void
		setCCInterruptLevel(uint8_t level)
		{
			TCC0_INTCTRLB = level;
		}
		
		static void
		setTimerCommand(uint8_t command, bool clear=false);
		
		inline static bool
		isTimerCountingUp()
		{
			return (!(TCC0_CTRLFSET & TC0_DIR_bm));
		}
		
		// specific configuration combinations
		
		/// Creates a medium level OVL interrupt every x ms. Accuracy not guaranteed!
		/// \param interval between interrupt, max 131ms at 32MHz, max 2097ms at 2MHz
		static void
		setMsTimer(uint8_t interval=1);
	};
}

#endif	// TCC0
#endif // XPCC__XMEGA_TIMER_C0_HPP