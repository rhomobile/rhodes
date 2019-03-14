#pragma once


namespace rho {
namespace ruby {

class IRunnable {

public:
    virtual ~IRunnable() {}

    virtual void run()=0;

};

}
}
