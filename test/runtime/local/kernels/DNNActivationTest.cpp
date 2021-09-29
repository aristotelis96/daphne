/*
 * Copyright 2021 The DAPHNE Consortium
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <runtime/local/datagen/GenGivenVals.h>
#include <runtime/local/datastructures/DenseMatrix.h>
#include <runtime/local/kernels/CheckEq.h>

#include <cassert>

#ifdef USE_CUDA
    #include <api/cli/DaphneUserConfig.h>
    #include <runtime/local/kernels/CUDA_Activation.h>
    #include <runtime/local/kernels/CUDA_InitContext.h>
#else
// ToDo: cpu version
//    #include <runtime/local/kernels/Activation.h>
#endif

#include <tags.h>
#include <catch.hpp>

template<class OP, class DT>
void check(const DT* in, const DT* exp, DaphneContext* dctx) {
    DT* res = nullptr;
#ifdef USE_CUDA
    Activation::Forward_CUDA<OP, DT, DT>::apply(res, in, dctx);
#else
    //"ToDo: cpu version
    return;
    Activation::Forward<OP, DT, DT>::apply(res, in, dctx);
#endif
    CHECK(*res == *exp);
}

TEMPLATE_PRODUCT_TEST_CASE("relu_fwd", TAG_DNN, (DenseMatrix), (float, double)) {
    using DT = TestType;

    auto dctx = new DaphneContext();
#ifdef USE_CUDA
    initCUDAContext(dctx);
#endif

    auto input = genGivenVals<DT>(1, { -3, -2, -1, 0, 1, 2, 3, 4, 5});

    // expected output when used with settings filter 2x2, stride 1x1, padding 0x0
    auto result = genGivenVals<DT>(1, { 0, 0, 0, 0, 1, 2, 3, 4, 5 });

    check<Activation::ReLU>(input, result, dctx);

    DataObjectFactory::destroy(input);
    DataObjectFactory::destroy(result);

    delete dctx;
}
