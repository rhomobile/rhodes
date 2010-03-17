/*
 *  SplashScreenImpl.cpp
 *  rhorunner
 *
 *  Created by Dmitry Moskalchuk on 11.03.10.
 *  Copyright 2010 Rhomobile. All rights reserved.
 *
 */

#include "SplashScreenImpl.h"

#include "common/RhodesApp.h"

extern "C"
{

void rho_splash_screen_start()
{
    RHODESAPP().getSplashScreen().start();
}

void rho_splash_screen_hide()
{
    RHODESAPP().getSplashScreen().hide();
}

}
