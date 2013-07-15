#pragma once

namespace sensormodule
{
	struct IRawSensor
	{
	    virtual int getMinimumGap() = 0;
        virtual void setMinimumGap(int minimumGap) = 0;
        virtual const char* getType() = 0;
        virtual const char* getStatus() = 0;

        virtual bool Start() = 0;
        virtual bool Stop() = 0;
	};
}