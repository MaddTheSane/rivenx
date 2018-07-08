//
//  platform_info.h
//  rivenx
//
//  Copyright (c) 2012 MacStorm. All rights reserved.
//

#pragma once

#include <Foundation/NSString.h>

__BEGIN_DECLS

extern NSString* copy_hardware_uuid(void) NS_RETURNS_RETAINED;
extern NSString* copy_product_type(void) NS_RETURNS_RETAINED;
extern NSString* copy_system_build(void) NS_RETURNS_RETAINED;
extern NSString* copy_system_version(void) NS_RETURNS_RETAINED;
extern NSString* copy_computer_name(void) NS_RETURNS_RETAINED;
extern NSString* copy_principal_mac_address(void) NS_RETURNS_RETAINED;

__END_DECLS
