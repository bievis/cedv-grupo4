#!/bin/bash

g++ -I. -g -Wall -pedantic prueba_xml.cpp GetXMLConfig.cpp -o parser -lxerces-c
