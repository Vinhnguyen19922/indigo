//  Copyright (c) 2016 CloudMakers, s. r. o.
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//
//  1. Redistributions of source code must retain the above copyright
//  notice, this list of conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above
//  copyright notice, this list of conditions and the following
//  disclaimer in the documentation and/or other materials provided
//  with the distribution.
//
//  3. The name of the author may not be used to endorse or promote
//  products derived from this software without specific prior
//  written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE AUTHOR 'AS IS' AND ANY EXPRESS
//  OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
//  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
//  GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
//  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

//  version history
//  2.0 Build 0 - PoC by Peter Polakovic <peter.polakovic@cloudmakers.eu>

#include <string.h>

#include "indigo_version.h"

// <getProperties version='1.7' name='CONFIG'/>

struct property_mapping {
  char *legacy;
  char *current;
  struct item_mapping {
    char *legacy;
    char *current;
  } items[10];
};

static struct property_mapping legacy[] = {
  { "DEBUG", "DEBUG", {
    { "ENABLE", "ENABLED" },
    { "DISABLE", "DISABLED" },
    NULL }
  },
  { "SIMULATION", "SIMULATION", {
    { "ENABLE", "ENABLED" },
    { "DISABLE", "DISABLED" },
    NULL }
  },
  { "CONFIG_PROCESS", "CONFIG", {
    { "CONFIG_LOAD", "LOAD" },
    { "CONFIG_SAVE", "SAVE" },
    { "CONFIG_DEFAULT", "DEFAULT" },
    NULL }
  },
  { "DRIVER_INFO", "DEVICE_INFO", {
    { "DRIVER_NAME", "NAME" },
    { "DRIVER_VERSION", "VERSION" },
    { "DRIVER_INTERFACE", "INTERFACE" },
    NULL }
  },
  { "CCD_INFO", "CCD_INFO", {
    { "CCD_MAX_X", "WIDTH" },
    { "CCD_MAX_Y", "HEIGHT" },
    { "CCD_MAX_BIN_X", "MAX_HORIZONAL_BIN" },
    { "CCD_MAX_BIN_Y", "MAX_VERTICAL_BIN" },
    { "CCD_PIXEL_SIZE", "PIXEL_SIZE" },
    { "CCD_PIXEL_SIZE_X", "PIXEL_WIDTH" },
    { "CCD_PIXEL_SIZE_Y", "PIXEL_HEIGHT" },
    { "CCD_BITSPERPIXEL", "BITS_PER_PIXEL" },
    NULL }
  },
  { "CCD_EXPOSURE", "CCD_EXPOSURE", {
    { "CCD_EXPOSURE_VALUE", "EXPOSURE" },
    NULL }
  },
  { "CCD_ABORT_EXPOSURE", "CCD_ABORT_EXPOSURE", {
    { "ABORT", "ABORT_EXPOSURE" },
    NULL }
  },
  { "CCD_FRAME", "CCD_FRAME", {
    { "X", "LEFT" },
    { "Y", "TOP" },
    NULL }
  },
  { "CCD_BINNING", "CCD_BIN", {
    { "VER_BIN", "HORIZONTAL" },
    { "HOR_BIN", "VERTICAL" },
    NULL }
  },
  { "CCD_FRAME_TYPE", "CCD_FRAME_TYPE", {
    { "FRAME_LIGHT", "LIGHT" },
    { "FRAME_BIAS", "BIAS" },
    { "FRAME_DARK", "DARK" },
    { "FRAME_FLAT", "FLAT" },
    NULL }
  },
  { "UPLOAD_MODE", "CCD_UPLOAD_MODE", {
    { "UPLOAD_CLIENT", "CLIENT" },
    { "UPLOAD_LOCAL", "LOCAL" },
    { "UPLOAD_BOTH", "BOTH" },
    NULL }
  },
  { "UPLOAD_SETTINGS", "CCD_LOCAL_MODE", {
    { "UPLOAD_DIR", "DIR" },
    { "UPLOAD_PREFIX", "PREFIX" },
    NULL }
  },
  { "CCD_FILE_PATH", "CCD_IMAGE_FILE", {
    { "FILE_PATH", "FILE" },
    NULL }
  },
  { "CCD1", "CCD_IMAGE", {
    { "CCD1", "IMAGE" },
    NULL }
  },
  { "CCD_TEMPERATURE", "CCD_TEMPERATURE", {
    { "CCD_TEMPERATURE_VALUE", "TEMPERATURE" },
    NULL }
  },
  { "CCD_COOLER", "CCD_COOLER", {
    { "COOLER_ON", "ON" },
    { "COOLER_OFF", "OFF" },
    NULL }
  },
  { "CCD_COOLER_POWER", "CCD_COOLER_POWER", {
    { "CCD_COOLER_VALUE", "POWER" },
    NULL }
  },
  { "TELESCOPE_TIMED_GUIDE_NS", "TELESCOPE_GUIDE_DEC", {
    { "TIMED_GUIDE_N", "TELESCOPE_GUIDE_NORTH" },
    { "TIMED_GUIDE_S", "TELESCOPE_GUIDE_SOUTH" },
    NULL }
  },
  { "TELESCOPE_TIMED_GUIDE_WE", "TELESCOPE_GUIDE_RA", {
    { "TIMED_GUIDE_W", "TELESCOPE_GUIDE_WEST" },
    { "TIMED_GUIDE_E", "TELESCOPE_GUIDE_EAST" },
    NULL }
  },
  NULL
};

void indigo_copy_property_name(indigo_version version, indigo_property *property, const char *name) {
  if (version == INDIGO_VERSION_LEGACY) {
    struct property_mapping *property_mapping = legacy;
    while (property_mapping->legacy) {
      if (!strcmp(name, property_mapping->legacy)) {
        INDIGO_DEBUG(indigo_debug("version: %s -> %s (current)", property_mapping->legacy, property_mapping->current));
        strcpy(property->name, property_mapping->current);
        return;
      }
      property_mapping++;
    }
  }
  strncpy(property->name, name, INDIGO_NAME_SIZE);
}

void indigo_copy_item_name(indigo_version version, indigo_property *property, indigo_item *item, const char *name) {
  if (version == INDIGO_VERSION_LEGACY) {
    struct property_mapping *property_mapping = legacy;
    while (property_mapping->legacy) {
      if (!strcmp(property->name, property_mapping->current)) {
        struct item_mapping *item_mapping = property_mapping->items;
        while (item_mapping->legacy) {
          if (!strcmp(name, item_mapping->legacy)) {
            INDIGO_DEBUG(indigo_debug("version: %s.%s -> %s.%s (current)", property_mapping->legacy, item_mapping->legacy, property_mapping->current, item_mapping->current));
            strncpy(item->name, item_mapping->current, INDIGO_NAME_SIZE);
            return;
          }
          item_mapping++;
        }
        return;
      }
      property_mapping++;
    }
  }
  strncpy(item->name, name, INDIGO_NAME_SIZE);
}

const char *indigo_property_name(indigo_version version, indigo_property *property) {
  if (version == INDIGO_VERSION_LEGACY) {
    struct property_mapping *property_mapping = legacy;
    while (property_mapping->legacy) {
      if (!strcmp(property->name, property_mapping->current)) {
        INDIGO_DEBUG(indigo_debug("version: %s -> %s (legacy)", property_mapping->current, property_mapping->legacy));
        return property_mapping->legacy;
      }
      property_mapping++;
    }
  }
  return property->name;
}

const char *indigo_item_name(indigo_version version, indigo_property *property, indigo_item *item) {
  if (version == INDIGO_VERSION_LEGACY) {
    struct property_mapping *property_mapping = legacy;
    while (property_mapping->legacy) {
      if (!strcmp(property->name, property_mapping->current)) {
        struct item_mapping *item_mapping = property_mapping->items;
        while (item_mapping->legacy) {
          if (!strcmp(item->name, item_mapping->current)) {
            INDIGO_DEBUG(indigo_debug("version: %s.%s -> %s.%s (legacy)", property_mapping->current, item_mapping->current, property_mapping->legacy, item_mapping->legacy));
            return item_mapping->legacy;
          }
          item_mapping++;
        }
        return item->name;
      }
      property_mapping++;
    }
  }
  return item->name;
}

