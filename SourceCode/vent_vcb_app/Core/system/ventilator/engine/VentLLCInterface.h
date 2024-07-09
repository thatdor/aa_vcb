/*
 * VentLLCInterface.h
 *
 *  Created on: Mar 25, 2024
 *      Author: dawei.zhou
 */

#ifndef SYSTEM_VENTILATOR_ENGINE_VENTLLCINTERFACE_H_
#define SYSTEM_VENTILATOR_ENGINE_VENTLLCINTERFACE_H_
#include "LLCParameters.h"

class VentLLCInterface
{
public:
    virtual void clockRun() = 0;

    virtual bool isPressureStable() const = 0;

    virtual bool isFlowStable() const = 0;

    virtual void NewSettings(LLCparameters &_settings) = 0;

};

#endif /* SYSTEM_VENTILATOR_ENGINE_VENTLLCINTERFACE_H_ */
