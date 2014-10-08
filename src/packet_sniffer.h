#pragma once

#include "globals.h"
#include "middleware.h"
#include "filter.h"

#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

void* sniff(void *val);
