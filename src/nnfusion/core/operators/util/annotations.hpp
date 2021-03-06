//*****************************************************************************
// Copyright 2017-2020 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//*****************************************************************************

// Microsoft (c) 2019, NNFusion Team

#pragma once

#include "nnfusion/util/util.hpp"

namespace nnfusion
{
    struct oi_pair
    {
        oi_pair(size_t out, size_t in, bool destruct)
            : output(out)
            , input(in)
            , destructive(destruct)
            , input_offset(0)
        {
        }
        oi_pair(size_t out, size_t in, bool destruct, size_t offset)
            : output(out)
            , input(in)
            , destructive(destruct)
            , input_offset(offset)
        {
        }
        size_t output;
        size_t input;
        bool destructive;
        size_t input_offset = 0;
    };

    /// \brief Base class for annotations added to graph ops or kernels.
    class Annotations
    {
    public:
        virtual ~Annotations() = default;

        void add_in_place_oi_pair(const struct oi_pair& oi)
        {
            for (auto e : m_in_place_oi_pairs)
            {
                if (e.input == oi.input && e.output == oi.output)
                {
                    if (e.destructive == oi.destructive)
                        return;
                    else
                        NNFUSION_CHECK_FAIL()
                            << "In_place hint destructive state conflicts with an existing entry. ";
                }

                // NNFUSION_CHECK(e.input != oi.input && e.output != oi.output)
                //     << "In_place hint conflicts with an existing entry";
            }
            m_in_place_oi_pairs.emplace_back(oi);
        }

        const std::vector<struct oi_pair>& get_in_place_oi_pairs() const
        {
            return m_in_place_oi_pairs;
        }

    private:
        // map of output-input pairs for which in-place computation is valid
        std::vector<struct oi_pair> m_in_place_oi_pairs;
    };
}
