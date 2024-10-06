/*
Author: Nilesh Ghule <nilesh@sunbeaminfo.com>
Course: PG-DESD @ Sunbeam Infotech
Date: Sep 30, 2024
*/

#include "rtc.h"

// input range = 0 to 99.
uint16_t Bcd2Bin(uint16_t bcd) {
	uint16_t units = bcd & 0x000F;
	uint16_t tens = (bcd >> 4) & 0x000F;
	uint16_t bin = tens * 10 + units;
	return bin;
}

// input range = 0 to 99.
uint16_t Bin2Bcd(uint16_t bin) {
	uint16_t units = bin % 10;
	uint16_t tens = (bin / 10) % 10;
	uint16_t bcd = (tens << 4) | units;
	return bcd;
}

void RTC_Init(RTC_Date *dt, RTC_Time *tm) {
	//* enable peripheral clock power
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	//* enable access to the RTC registers in PWR control (stm32 manual 5.4)
	PWR->CR |= PWR_CR_DBP;
	//* enable LSI in RCC CSR (stm32 manual 7.3.21)
	RCC->CSR |= RCC_CSR_LSION;
	//* wait for LSI ready flag
	while(!(RCC->CSR & RCC_CSR_LSIRDY));
	//* select LSI (32 KHz) and enable RTC in RCC (BDCR) (stm32 manual 7.3.21)
	RCC->BDCR |= RCC_BDCR_RTCSEL_1 | RCC_BDCR_RTCEN;
	//* enter key to unlock write protection. key sequence must be 0xCA and then 0x53 (stm32 manual 26.3.5)
	RTC->WPR = 0xCA;
	RTC->WPR = 0x53;
	//* enable rtc setup in RTC ISR (i.e. register init)
	RTC->ISR |= RTC_ISR_INIT;
	//* wait until register update is allowed
	while(!(RTC->ISR & RTC_ISR_INITF));
	//* set sync & async prescalar in RTC PRER
	// For 1 Hz internal clock and LSI = 32000 Hz,
	//	Async Prescalar = 128 (-1) and Sync Prescalar = 250 (-1)
	RTC->PRER = ((128-1) << RTC_PRER_PREDIV_A_Pos) | ((250-1) << RTC_PRER_PREDIV_S_Pos);
	//* set initial date and time
	RTC_SetDate(dt);
	RTC_SetTime(tm);
	//* rtc date/time should be read from calendar registers (not shadow registers) (RTC CR)
	RTC->CR |= RTC_CR_BYPSHAD;
	//* start rtc in free running mode (RTC ISR)
	RTC->ISR &= ~RTC_ISR_INIT;
	//* disable access to RTC registers (write protection) in PWR control
	PWR->CR &= ~PWR_CR_DBP;
}

void RTC_SetDate(RTC_Date *dt) {
	uint16_t date = Bin2Bcd(dt->Date);
	uint16_t mon = Bin2Bcd(dt->Month);
	uint16_t yr = Bin2Bcd(dt->Year);
	uint16_t day = Bin2Bcd(dt->WeekDay);
	uint32_t dr = ((uint32_t)date << RTC_DR_DU_Pos)
			| ((uint32_t)mon << RTC_DR_MU_Pos)
			| ((uint32_t)yr << RTC_DR_YU_Pos)
			| ((uint32_t)day << RTC_DR_WDU_Pos);
	RTC->DR = dr;
}

void RTC_SetTime(RTC_Time *tm) {
	uint16_t hr = Bin2Bcd(tm->Hour);
	uint16_t min = Bin2Bcd(tm->Minute);
	uint16_t sec = Bin2Bcd(tm->Second);
	uint32_t tr = ((uint32_t)hr << RTC_TR_HU_Pos)
			| ((uint32_t)min << RTC_TR_MNU_Pos)
			| ((uint32_t)sec << RTC_TR_SU_Pos);
	RTC->TR = tr;
}

void RTC_GetDate(RTC_Date *dt) {
	uint32_t dr = RTC->DR;
	uint16_t year = (dr & (RTC_DR_YT_Msk|RTC_DR_YU_Msk)) >> RTC_DR_YU_Pos;
	uint16_t mon = (dr & (RTC_DR_MT_Msk|RTC_DR_MU_Msk)) >> RTC_DR_MU_Pos;
	uint16_t date = (dr & (RTC_DR_DT_Msk|RTC_DR_DU_Msk)) >> RTC_DR_DU_Pos;
	uint16_t day = (dr & RTC_DR_WDU_Msk) >> RTC_DR_WDU_Pos;
	dt->Year = Bcd2Bin(year);
	dt->Month = Bcd2Bin(mon);
	dt->Date = Bcd2Bin(date);
	dt->WeekDay = Bcd2Bin(day);
}

void RTC_GetTime(RTC_Time *tm) {
	uint32_t tr = RTC->TR;
	uint16_t hr = (tr & (RTC_TR_HT_Msk|RTC_TR_HU_Msk)) >> RTC_TR_HU_Pos;
	uint16_t min = (tr & (RTC_TR_MNT_Msk|RTC_TR_MNU_Msk)) >> RTC_TR_MNU_Pos;
	uint16_t sec = (tr & (RTC_TR_ST_Msk|RTC_TR_SU_Msk)) >> RTC_TR_SU_Pos;
	tm->Hour = Bcd2Bin(hr);
	tm->Minute = Bcd2Bin(min);
	tm->Second = Bcd2Bin(sec);
}










