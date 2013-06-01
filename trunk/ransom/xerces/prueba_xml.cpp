#include <iostream>
#include <string>
#include "GetXMLConfig.h"

using namespace std;

int main()
{
   string configFile = "sample.xml"; // stat file. Get ambigious segfault otherwise.

   GetXMLConfig::getSingleton().readConfigFile ( configFile );

   cout << "Application option A = " << GetXMLConfig::getSingleton().getOptionA() << endl;
   cout << "Application option B = " << GetXMLConfig::getSingleton().getOptionB() << endl;

   return 0;
}

