
// This header pulls in llvm/ADT/OwningPtr.h

// License as per the original source.

#pragma once

#include "detail/OwningPtr.h"

namespace sax {

template<typename T>
using owningptr = llvm::OwningPtr<T>;

}
