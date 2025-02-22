/*
 * Copyright 2019 Google Inc.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "util/proto_util.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <string>

#include "util/file_test.pb.h"
#include "util/status_testing.inc"

namespace private_join_and_compute {

namespace {
using testing::TestProto;

TEST(ProtoUtilsTest, ConvertsToAndFrom) {
  TestProto expected_test_proto;
  expected_test_proto.set_record("data");
  expected_test_proto.set_dummy("dummy");
  std::string serialized = ProtoUtils::ToString(expected_test_proto);
  TestProto actual_test_proto = ProtoUtils::FromString<TestProto>(serialized);
  EXPECT_EQ(actual_test_proto.record(), expected_test_proto.record());
  EXPECT_EQ(actual_test_proto.dummy(), expected_test_proto.dummy());
}

TEST(ProtoUtilsTest, ReadWriteToFile) {
  std::filesystem::path temp_dir(::testing::TempDir());
  std::string filename = (temp_dir / "proto_file").string();

  TestProto expected_test_proto;
  expected_test_proto.set_record("data");
  expected_test_proto.set_dummy("dummy");

  ASSERT_TRUE(ProtoUtils::WriteProtoToFile(expected_test_proto, filename).ok());
  ASSERT_OK_AND_ASSIGN(TestProto actual_test_proto,
                       ProtoUtils::ReadProtoFromFile<TestProto>(filename));
  EXPECT_EQ(actual_test_proto.record(), expected_test_proto.record());
  EXPECT_EQ(actual_test_proto.dummy(), expected_test_proto.dummy());
}

}  // namespace

}  // namespace private_join_and_compute
