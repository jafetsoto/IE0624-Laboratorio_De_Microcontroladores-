#!/bin/bash
socat PTY,link=/tmp/ttyS1,raw,echo=0 PTY,link=/tmp/ttyS2,raw,echo=0
