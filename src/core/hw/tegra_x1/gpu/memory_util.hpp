#pragma once

#include "core/hw/tegra_x1/gpu/const.hpp"

namespace hydra::hw::tegra_x1::gpu {

void ConvertBlockLinearToLinear(u32 stride, u32 rows, u32 block_height_log2,
                                const u8* in_data, u8* out_data);
void ConvertLinearToBlockLinear(u32 stride, u32 rows, u32 block_height_log2,
                                const u8* in_data, u8* out_data);

} // namespace hydra::hw::tegra_x1::gpu
