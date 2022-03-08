/*
 * Copyright 2022 The DAPHNE Consortium
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

#include <runtime/local/datastructures/DataObjectFactory.h>
#include <runtime/local/datastructures/DenseMatrix.h>
#include <runtime/local/io/ReadMM.h>

#include <tags.h>

#include <catch.hpp>

#include <vector>

#include <cmath>
#include <cstdint>
#include <limits>

TEMPLATE_PRODUCT_TEST_CASE("ReadMM CIG", TAG_KERNELS, (DenseMatrix), (int32_t)) {
  using DT = TestType;
  DT *m = nullptr;

  size_t numRows = 9;
  size_t numCols = 9;

  char filename[] = "./test/runtime/local/io/cig.mtx";
  readMM(m, filename);

  REQUIRE(m->getNumRows() == numRows);
  REQUIRE(m->getNumCols() == numCols);

  CHECK(m->get(0, 0) == 1);
  CHECK(m->get(2, 0) == 0);
  CHECK(m->get(3, 4) == 9);
  CHECK(m->get(7, 4) == 4);

  DataObjectFactory::destroy(m);
}

TEMPLATE_PRODUCT_TEST_CASE("ReadMM AIG", TAG_KERNELS, (DenseMatrix), (int32_t)) {
  using DT = TestType;
  DT *m = nullptr;

  size_t numRows = 4;
  size_t numCols = 3;

  char filename[] = "./test/runtime/local/io/aig.mtx";
  readMM(m, filename);

  REQUIRE(m->getNumRows() == numRows);
  REQUIRE(m->getNumCols() == numCols);

  CHECK(m->get(0, 0) == 1);
  CHECK(m->get(1, 0) == 2);
  CHECK(m->get(0, 1) == 5);
  CHECK(m->get(3, 2) == 12);
  CHECK(m->get(2, 1) == 7);

  DataObjectFactory::destroy(m);
}

TEMPLATE_PRODUCT_TEST_CASE("ReadMM CRG", TAG_KERNELS, (DenseMatrix), (double)) {
  using DT = TestType;
  DT *m = nullptr;

  size_t numRows = 497;
  size_t numCols = 507;

  char filename[] = "./test/runtime/local/io/crg.mtx";
  readMM(m, filename);

  REQUIRE(m->getNumRows() == numRows);
  REQUIRE(m->getNumCols() == numCols);

  CHECK(m->get(5, 0) == 0.25599762);
  CHECK(m->get(6, 0) == 0.13827993);
  CHECK(m->get(200, 4) == 0.20001954);

  DataObjectFactory::destroy(m);
}

TEMPLATE_PRODUCT_TEST_CASE("ReadMM CRS", TAG_KERNELS, (DenseMatrix), (double)) {
  using DT = TestType;
  DT *m = nullptr;

  size_t numRows = 66;
  size_t numCols = 66;

  char filename[] = "./test/runtime/local/io/crs.mtx";
  readMM(m, filename);

  REQUIRE(m->getNumRows() == numRows);
  REQUIRE(m->getNumCols() == numCols);

  CHECK(m->get(36, 29) == 926.188986068);

  for(int r = 0; r<numRows; r++)
    for(int c = r+1; c<numCols; c++)
      CHECK(m->get(r,c) == m->get(c,r));

  DataObjectFactory::destroy(m);
}

TEMPLATE_PRODUCT_TEST_CASE("ReadMM CRK", TAG_KERNELS, (DenseMatrix), (double)) {
  using DT = TestType;
  DT *m = nullptr;

  size_t numRows = 66;
  size_t numCols = 66;

  char filename[] = "./test/runtime/local/io/crk.mtx";
  readMM(m, filename);

  REQUIRE(m->getNumRows() == numRows);
  REQUIRE(m->getNumCols() == numCols);

  CHECK(m->get(29, 36) == -926.188986068);

  for(int r = 0; r<numRows; r++) {
    CHECK(m->get(r,r) == 0);
    for(int c = r+1; c<numCols; c++)
      CHECK(m->get(r,c) == -m->get(c,r));
  }

  DataObjectFactory::destroy(m);
}

TEMPLATE_PRODUCT_TEST_CASE("ReadMM CPS", TAG_KERNELS, (DenseMatrix), (int32_t)) {
  using DT = TestType;
  DT *m = nullptr;

  size_t numRows = 24;
  size_t numCols = 24;

  char filename[] = "./test/runtime/local/io/cps.mtx";
  readMM(m, filename);

  REQUIRE(m->getNumRows() == numRows);
  REQUIRE(m->getNumCols() == numCols);

  CHECK(m->get( 0, 0) != 0);
  CHECK(m->get( 1, 0) == 0);
  CHECK(m->get(3, 15) != 0);

  for(int r = 0; r<numRows; r++)
    for(int c = r+1; c<numCols; c++)
      if(m->get(r,c) == 0)
        CHECK(m->get(c,r) == 0);
      else
        CHECK(m->get(c,r) != 0);

  DataObjectFactory::destroy(m);
}

TEMPLATE_PRODUCT_TEST_CASE("ReadMM AIK", TAG_KERNELS, (DenseMatrix), (int32_t)) {
  using DT = TestType;
  DT *m = nullptr;

  size_t numRows = 4;
  size_t numCols = 4;

  char filename[] = "./test/runtime/local/io/aik.mtx";
  readMM(m, filename);

  REQUIRE(m->getNumRows() == numRows);
  REQUIRE(m->getNumCols() == numCols);

  CHECK(m->get(1, 0) == 1);

  for(int r = 0; r<numRows; r++) {
    CHECK(m->get(r,r) == 0);
    for(int c = r+1; c<numCols; c++)
      CHECK(m->get(r,c) == -m->get(c,r));
  }

  DataObjectFactory::destroy(m);
}

TEMPLATE_PRODUCT_TEST_CASE("ReadMM AIS", TAG_KERNELS, (DenseMatrix), (int32_t)) {
  using DT = TestType;
  DT *m = nullptr;

  size_t numRows = 3;
  size_t numCols = 3;

  char filename[] = "./test/runtime/local/io/ais.mtx";
  readMM(m, filename);

  REQUIRE(m->getNumRows() == numRows);
  REQUIRE(m->getNumCols() == numCols);

  CHECK(m->get(1, 1) == 4);

  for(int r = 0; r<numRows; r++)
    for(int c = r+1; c<numCols; c++)
      CHECK(m->get(r,c) == m->get(c,r));

  DataObjectFactory::destroy(m);
}

TEMPLATE_PRODUCT_TEST_CASE("ReadMM CIG (CSR)", TAG_KERNELS, (CSRMatrix), (int32_t)) {
  using DT = TestType;
  DT *m = nullptr;

  size_t numRows = 9;
  size_t numCols = 9;

  char filename[] = "./test/runtime/local/io/cig.mtx";
  readMM(m, filename);

  REQUIRE(m->getNumRows() == numRows);
  REQUIRE(m->getNumCols() == numCols);

  CHECK(m->get(0, 0) == 1);
  CHECK(m->get(2, 0) == 0);
  CHECK(m->get(3, 4) == 9);
  CHECK(m->get(7, 4) == 4);

  DataObjectFactory::destroy(m);
}

TEMPLATE_PRODUCT_TEST_CASE("ReadMM AIG (CSR)", TAG_KERNELS, (CSRMatrix), (int32_t)) {
  using DT = TestType;
  DT *m = nullptr;

  size_t numRows = 4;
  size_t numCols = 3;

  char filename[] = "./test/runtime/local/io/aig.mtx";
  readMM(m, filename);

  REQUIRE(m->getNumRows() == numRows);
  REQUIRE(m->getNumCols() == numCols);

  CHECK(m->get(0, 0) == 1);
  CHECK(m->get(1, 0) == 2);
  CHECK(m->get(0, 1) == 5);
  CHECK(m->get(3, 2) == 12);
  CHECK(m->get(2, 1) == 7);

  DataObjectFactory::destroy(m);
}

TEMPLATE_PRODUCT_TEST_CASE("ReadMM CRG (CSR)", TAG_KERNELS, (CSRMatrix), (double)) {
  using DT = TestType;
  DT *m = nullptr;

  size_t numRows = 497;
  size_t numCols = 507;

  char filename[] = "./test/runtime/local/io/crg.mtx";
  readMM(m, filename);

  REQUIRE(m->getNumRows() == numRows);
  REQUIRE(m->getNumCols() == numCols);

  CHECK(m->get(5, 0) == 0.25599762);
  CHECK(m->get(6, 0) == 0.13827993);
  CHECK(m->get(200, 4) == 0.20001954);

  DataObjectFactory::destroy(m);
}

TEMPLATE_PRODUCT_TEST_CASE("ReadMM CRS (CSR)", TAG_KERNELS, (CSRMatrix), (double)) {
  using DT = TestType;
  DT *m = nullptr;

  size_t numRows = 66;
  size_t numCols = 66;

  char filename[] = "./test/runtime/local/io/crs.mtx";
  readMM(m, filename);

  REQUIRE(m->getNumRows() == numRows);
  REQUIRE(m->getNumCols() == numCols);

  CHECK(m->get(36, 29) == 926.188986068);

  for(int r = 0; r<numRows; r++)
    for(int c = r+1; c<numCols; c++)
      CHECK(m->get(r,c) == m->get(c,r));

  DataObjectFactory::destroy(m);
}

TEMPLATE_PRODUCT_TEST_CASE("ReadMM CRK (CSR)", TAG_KERNELS, (CSRMatrix), (double)) {
  using DT = TestType;
  DT *m = nullptr;

  size_t numRows = 66;
  size_t numCols = 66;

  char filename[] = "./test/runtime/local/io/crk.mtx";
  readMM(m, filename);

  REQUIRE(m->getNumRows() == numRows);
  REQUIRE(m->getNumCols() == numCols);

  CHECK(m->get(29, 36) == -926.188986068);

  for(int r = 0; r<numRows; r++) {
    CHECK(m->get(r,r) == 0);
    for(int c = r+1; c<numCols; c++)
      CHECK(m->get(r,c) == -m->get(c,r));
  }

  DataObjectFactory::destroy(m);
}

TEMPLATE_PRODUCT_TEST_CASE("ReadMM CPS (CSR)", TAG_KERNELS, (CSRMatrix), (int32_t)) {
  using DT = TestType;
  DT *m = nullptr;

  size_t numRows = 24;
  size_t numCols = 24;

  char filename[] = "./test/runtime/local/io/cps.mtx";
  readMM(m, filename);

  REQUIRE(m->getNumRows() == numRows);
  REQUIRE(m->getNumCols() == numCols);

  CHECK(m->get( 0, 0) != 0);
  CHECK(m->get( 1, 0) == 0);
  CHECK(m->get(3, 15) != 0);

  for(int r = 0; r<numRows; r++)
    for(int c = r+1; c<numCols; c++)
      if(m->get(r,c) == 0)
        CHECK(m->get(c,r) == 0);
      else
        CHECK(m->get(c,r) != 0);

  DataObjectFactory::destroy(m);
}

TEMPLATE_PRODUCT_TEST_CASE("ReadMM AIK (CSR)", TAG_KERNELS, (CSRMatrix), (int32_t)) {
  using DT = TestType;
  DT *m = nullptr;

  size_t numRows = 4;
  size_t numCols = 4;

  char filename[] = "./test/runtime/local/io/aik.mtx";
  readMM(m, filename);

  REQUIRE(m->getNumRows() == numRows);
  REQUIRE(m->getNumCols() == numCols);

  CHECK(m->get(1, 0) == 1);

  for(int r = 0; r<numRows; r++) {
    CHECK(m->get(r,r) == 0);
    for(int c = r+1; c<numCols; c++)
      CHECK(m->get(r,c) == -m->get(c,r));
  }

  DataObjectFactory::destroy(m);
}

TEMPLATE_PRODUCT_TEST_CASE("ReadMM AIS (CSR)", TAG_KERNELS, (CSRMatrix), (int32_t)) {
  using DT = TestType;
  DT *m = nullptr;

  size_t numRows = 3;
  size_t numCols = 3;

  char filename[] = "./test/runtime/local/io/ais.mtx";
  readMM(m, filename);

  REQUIRE(m->getNumRows() == numRows);
  REQUIRE(m->getNumCols() == numCols);

  CHECK(m->get(1, 1) == 4);

  for(int r = 0; r<numRows; r++)
    for(int c = r+1; c<numCols; c++)
      CHECK(m->get(r,c) == m->get(c,r));

  DataObjectFactory::destroy(m);
}