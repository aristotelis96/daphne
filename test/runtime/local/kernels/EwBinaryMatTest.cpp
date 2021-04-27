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

#include <runtime/local/datastructures/CSRMatrix.h>
#include <runtime/local/datagen/GenGivenVals.h>
#include <runtime/local/datastructures/DenseMatrix.h>
#include <runtime/local/kernels/CheckEq.h>
#include <runtime/local/kernels/EwBinaryMat.h>

#include <tags.h>

#include <catch.hpp>

#include <vector>

#include <cstdint>

#define TEST_NAME(opName) "EwBinaryMat (" opName ")"
#define DATA_TYPES DenseMatrix, CSRMatrix
#define VALUE_TYPES double, uint32_t

template<class DT>
void checkEwBinaryMat(BinaryOpCode opCode, const DT * lhs, const DT * rhs, const DT * exp) {
    DT * res = nullptr;
    ewBinaryMat<DT, DT, DT>(opCode, res, lhs, rhs);
    CHECK(*res == *exp);
}

TEMPLATE_PRODUCT_TEST_CASE(TEST_NAME("add"), TAG_KERNELS, (DATA_TYPES), (VALUE_TYPES)) {
    using DT = TestType;
    
    auto m0 = genGivenVals<DT>(4, {
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
    });
    auto m1 = genGivenVals<DT>(4, {
            1, 2, 0, 0, 1, 3,
            0, 1, 0, 2, 0, 3,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
    });
    auto m2 = genGivenVals<DT>(4, {
            0, 0, 0, 0, 0, 0,
            1, 2, 3, 1, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 3, 1, 0, 2,
    });
    auto m3 = genGivenVals<DT>(4, {
            1, 2, 0, 0, 1, 3,
            1, 3, 3, 3, 0, 3,
            0, 0, 0, 0, 0, 0,
            0, 0, 3, 1, 0, 2,
    });
    
    checkEwBinaryMat(BinaryOpCode::ADD, m0, m0, m0);
    checkEwBinaryMat(BinaryOpCode::ADD, m1, m0, m1);
    checkEwBinaryMat(BinaryOpCode::ADD, m1, m2, m3);
    
    DataObjectFactory::destroy(m0);
    DataObjectFactory::destroy(m1);
    DataObjectFactory::destroy(m2);
    DataObjectFactory::destroy(m3);
}

TEMPLATE_PRODUCT_TEST_CASE(TEST_NAME("mul"), TAG_KERNELS, (DATA_TYPES), (VALUE_TYPES)) {
    using DT = TestType;
    
    auto m0 = genGivenVals<DT>(4, {
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
    });
    auto m1 = genGivenVals<DT>(4, {
            1, 2, 0, 0, 1, 3,
            0, 1, 0, 2, 0, 3,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
    });
    auto m2 = genGivenVals<DT>(4, {
            0, 0, 0, 0, 0, 0,
            1, 2, 3, 1, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 3, 1, 0, 2,
    });
    auto m3 = genGivenVals<DT>(4, {
            0, 0, 0, 0, 0, 0,
            0, 2, 0, 2, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
    });
    
    checkEwBinaryMat(BinaryOpCode::MUL, m0, m0, m0);
    checkEwBinaryMat(BinaryOpCode::MUL, m1, m0, m0);
    checkEwBinaryMat(BinaryOpCode::MUL, m1, m2, m3);
        
    DataObjectFactory::destroy(m0);
    DataObjectFactory::destroy(m1);
    DataObjectFactory::destroy(m2);
    DataObjectFactory::destroy(m3);
}

TEMPLATE_PRODUCT_TEST_CASE(TEST_NAME("some invalid op-code"), TAG_KERNELS, (DATA_TYPES), (VALUE_TYPES)) {
    using DT = TestType;
    DT * res = nullptr;
    auto m = genGivenVals<DT>(1, {1});
    CHECK_THROWS(ewBinaryMat<DT, DT, DT>(static_cast<BinaryOpCode>(999), res, m, m));
}