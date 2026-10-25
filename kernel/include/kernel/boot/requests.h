#ifndef KERNEL_BOOT_REQUESTS_H_
#define KERNEL_BOOT_REQUESTS_H_

#include "kernel/lib/types.h"
#include <limine.h>
#include <array>

namespace requests
{
    struct BaseRevision {
        std::array<u64, 3> values;

        constexpr bool supported() const
        {
            return LIMINE_BASE_REVISION_SUPPORTED(values);
        }
    };

    BaseRevision base_revision();
} // namespace requests

#endif // KERNEL_BOOT_REQUESTS_H_