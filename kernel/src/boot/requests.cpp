#include "kernel/boot/requests.h"
#include "kernel/lib/types.h"
#include <limine.h>

namespace requests
{
    namespace
    {
        [[gnu::used, gnu::section(".limine_requests")]]
        volatile u64 limine_base_revision[] = LIMINE_BASE_REVISION(6);

        [[gnu::used, gnu::section(".limine_requests_start")]]
        volatile u64 limine_requests_start_marker[] = LIMINE_REQUESTS_START_MARKER;

        [[gnu::used, gnu::section(".limine_requests_end")]]
        volatile u64 limine_requests_end_marker[] = LIMINE_REQUESTS_END_MARKER;
    } // anonymous namespace

    BaseRevision base_revision()
    {
        return {{
            limine_base_revision[0],
            limine_base_revision[1],
            limine_base_revision[2]
        }};
    }
} // namespace requests