#pragma once

#ifdef __OBJC__
#import <Foundation/Foundation.h>
#endif

namespace hydra {

#ifndef __OBJC__
typedef void* id;
#endif

} // namespace hydra
