#pragma once


namespace rho {
namespace ruby {

class IRunnable {

public:
    virtual ~IRunnable() = 0;

    virtual void run()=0;

};

}
}
