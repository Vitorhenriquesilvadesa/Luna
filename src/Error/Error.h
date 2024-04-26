//
// Created by vitor on 24/04/24.
//

#ifndef LUNALANG_ERROR_H
#define LUNALANG_ERROR_H

namespace Luna::Internal::Error {

#include <iostream>

#define ERROR(x) std::cerr << x << std::endl; \
    exit(EXIT_FAILURE);

#define RETURN(message, code) std::cerr << message << std::endl; \
exit(code);

} // Internal
// Luna

#endif //LUNALANG_ERROR_H
