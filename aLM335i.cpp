#include "aLM335i.h"

/* aLM335 Copyright 2011 Kes Calhoun
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

    Redistributions of source code must retain the above copyright notice,
	this list of conditions and the following disclaimer.
    
    Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.
    
  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

aLM335i::aLM335i() {
  _pin = 0;
  _refVoltage = 50	;
}

aLM335i::aLM335i(int input) {
  _pin = input;
  _refVoltage = 50;
}

aLM335i::aLM335i(int input, int voltage) {
  _pin = input;
  _refVoltage = voltage;
}

aLM335i::aLM335i(int input, int voltage, bool externalReference) {
  _pin = input;
  if (externalReference) {
    analogReference(EXTERNAL);
  }
  else {
    analogReference(DEFAULT);
  }
  _refVoltage = voltage;
}

int aLM335i::_factorVoltage(int temperature) {

  if ( _refVoltage != 50 ) {
    temperature *= _refVoltage;
	temperature /= 50;
  }

  return (temperature);
}

	
int aLM335i::getRaw() {

  return (analogRead(_pin));

}

int aLM335i::getKelvin() {

  // The general formula for this is temperature = reading * 500/1023.
  // However, we cheat in a few places, by using 125/256 in order to make
  // the math a lot less ugly, at the cost of an unnoticably bit less accuracy.
  // Also, due to integer overflow and underflow, we have to bounce our
  // multiplications and divisions to stay under 32767, yet high enough to give
  // reasonable accuracy, which shaves off a bit more accuracy. But, hey, it's
  // tiny and fast.

  int temperature = getRaw();
  temperature *= 25;
  temperature /= 8;
  temperature *= 5;
  temperature /= 32;
  temperature = _factorVoltage(temperature);  
  return (temperature);

}

int aLM335i::getCelsius() {

  int temperature = getKelvin();
  temperature -= 273;

  return (temperature);

}

int aLM335i::getFahrenheit() {

  int temperature = getRaw();

  temperature = _factorVoltage(temperature);

  // The general formula for this is temperature = reading * 900/1023 - 460.
  // However, we cheat in a few places, by dividing by 1024 in order to make
  // the math a lot less ugly, at the cost of a near unnoticably slight bit
  // less accuracy. Also, due to integer overflow and underflow, we have to
  // bounce our multiplications and divisions to stay under 32767, yet high
  // enough to give reasonable accuracy.
  
  temperature *= 25;
  temperature /= 16;
  temperature *= 9;
  temperature /= 16;
  temperature -= 460;

  return (temperature);

}
