// coding: utf-8
/* Copyright (c) 2014, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef XPCC_BMP085_HPP
#	error  "Don't include this file directly, use 'bmp085.hpp' instead!"
#endif
#include <xpcc/math/utils/operator.hpp>
#include <xpcc/math/utils/bit_operation.hpp>

// ----------------------------------------------------------------------------
template < typename I2cMaster >
xpcc::Bmp085<I2cMaster>::Bmp085(DataBase &data, uint8_t address) :
	I2cDevice<I2cMaster, 1>(address), data(data)
{
}

// ----------------------------------------------------------------------------
// MARK: - Tasks
template < typename I2cMaster >
xpcc::ResumableResult<bool>
xpcc::Bmp085<I2cMaster>::initialize(Mode mode)
{
	RF_BEGIN();

	setMode(mode);
	buffer[0] = i(Register::CAL_AC1);

	this->transaction.configureWriteRead(buffer, 1, reinterpret_cast<uint8_t*>(&data.calibration), 22);

	if (RF_CALL( this->runTransaction() ))
	{
		uint16_t* element = reinterpret_cast<uint16_t*>(&data.calibration);
		element[ 0] = xpcc::fromBigEndian(element[0]);
		element[ 1] = xpcc::fromBigEndian(element[1]);
		element[ 2] = xpcc::fromBigEndian(element[2]);
		element[ 3] = xpcc::fromBigEndian(element[3]);
		element[ 4] = xpcc::fromBigEndian(element[4]);
		element[ 5] = xpcc::fromBigEndian(element[5]);

		element[ 6] = xpcc::fromBigEndian(element[6]);
		element[ 7] = xpcc::fromBigEndian(element[7]);

		element[ 8] = xpcc::fromBigEndian(element[8]);
		element[ 9] = xpcc::fromBigEndian(element[9]);
		element[10] = xpcc::fromBigEndian(element[10]);

		RF_RETURN(true);
	}

	RF_END_RETURN(false);
}

template < typename I2cMaster >
xpcc::ResumableResult<bool>
xpcc::Bmp085<I2cMaster>::readout()
{
	RF_BEGIN();

	// Start temperature reading
	buffer[0] = i(Register::CONTROL);
	buffer[1] = i(Conversion::Temperature);

	this->transaction.configureWrite(buffer, 2);

	if (not RF_CALL( this->runTransaction() ))
		RF_RETURN(false);

	// Wait until temperature reading is succeeded
	timeout.restart(5);
	RF_WAIT_UNTIL(timeout.isExpired());

	// Get the temperature from sensor
	buffer[0] = i(Register::MSB);
	this->transaction.configureWriteRead(buffer, 1, data.raw, 2);

	// Notify data class about changed buffer.
	data.rawTemperatureTouched();

	if (not RF_CALL( this->runTransaction() ))
		RF_RETURN(false);

	// buffer the mode for the timer later
	bufferedMode = data.meta & i(Mode::Mask);
	// Now start converting the pressure
	buffer[0] = i(Register::CONTROL);
	buffer[1] = i(Conversion::Pressure) | bufferedMode;

	this->transaction.configureWrite(buffer, 2);

	if (not RF_CALL( this->runTransaction() ))
		RF_RETURN(false);

	// Wait until sensor has converted the pressure
	timeout.restart(conversionDelay[bufferedMode >> 6]);
	RF_WAIT_UNTIL(timeout.isExpired());

	// Get the pressure from sensor
	buffer[0] = i(Register::MSB);
	this->transaction.configureWriteRead(buffer, 1, data.raw + 2, 3);

	// Notify data class about changed buffer.
	data.rawPressureTouched();

	RF_END_RETURN_CALL( this->runTransaction() );
}

template < typename I2cMaster >
constexpr uint8_t
xpcc::Bmp085< I2cMaster >::conversionDelay[];
