/**
 *  CONF.C
 *  =========
 *  (c) Copyright Michael 2013 
 *  Email: hdm258i@gmail.com 
 */

#include "conf.h"

static int debug = 0;

void Conf_Enable_Debug(){
    debug = 1;
}

void Conf_Disable_Debug(){
    debug = 0;
}

int Conf_Is_Debug(){
    return debug;
}
