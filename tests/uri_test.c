/*
 * Copyright 2010-2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */
#include <aws/io/uri.h>
#include <aws/testing/aws_test_harness.h>

static int s_test_uri_full_parse(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;
    const char *str_uri =
        "https://www.test.com:8443/path/to/resource?test1=value1&test%20space=value%20space&test2=value2&test2=value3";

    struct aws_byte_cursor uri_csr = aws_byte_cursor_from_c_str(str_uri);
    struct aws_uri uri;
    ASSERT_SUCCESS(aws_uri_init_parse(&uri, allocator, &uri_csr));

    struct aws_byte_cursor expected_scheme = aws_byte_cursor_from_c_str("https");
    ASSERT_BIN_ARRAYS_EQUALS(expected_scheme.ptr, expected_scheme.len, uri.scheme.ptr, uri.scheme.len);

    struct aws_byte_cursor expected_authority = aws_byte_cursor_from_c_str("www.test.com:8443");
    ASSERT_BIN_ARRAYS_EQUALS(expected_authority.ptr, expected_authority.len, uri.authority.ptr, uri.authority.len);

    struct aws_byte_cursor expected_host = aws_byte_cursor_from_c_str("www.test.com");
    ASSERT_BIN_ARRAYS_EQUALS(expected_host.ptr, expected_host.len, uri.host_name.ptr, uri.host_name.len);

    ASSERT_UINT_EQUALS(8443, uri.port);

    struct aws_byte_cursor expected_path = aws_byte_cursor_from_c_str("/path/to/resource");
    ASSERT_BIN_ARRAYS_EQUALS(expected_path.ptr, expected_path.len, uri.path.ptr, uri.path.len);

    struct aws_byte_cursor expected_query_str =
        aws_byte_cursor_from_c_str("test1=value1&test%20space=value%20space&test2=value2&test2=value3");
    ASSERT_BIN_ARRAYS_EQUALS(
        expected_query_str.ptr, expected_query_str.len, uri.query_string.ptr, uri.query_string.len);

    struct aws_byte_cursor expected_request_uri = aws_byte_cursor_from_c_str(
        "/path/to/resource?test1=value1&test%20space=value%20space&test2=value2&test2=value3");
    ASSERT_BIN_ARRAYS_EQUALS(
        expected_request_uri.ptr, expected_request_uri.len, uri.path_and_query.ptr, uri.path_and_query.len);

    aws_uri_clean_up(&uri);
    return AWS_OP_SUCCESS;
}

AWS_TEST_CASE(uri_full_parse, s_test_uri_full_parse);

static int s_test_uri_no_scheme_parse(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;
    const char *str_uri =
        "www.test.com:8443/path/to/resource?test1=value1&test%20space=value%20space&test2=value2&test2=value3";

    struct aws_byte_cursor uri_csr = aws_byte_cursor_from_c_str(str_uri);
    struct aws_uri uri;
    ASSERT_SUCCESS(aws_uri_init_parse(&uri, allocator, &uri_csr));

    ASSERT_UINT_EQUALS(0u, uri.scheme.len);

    struct aws_byte_cursor expected_authority = aws_byte_cursor_from_c_str("www.test.com:8443");
    ASSERT_BIN_ARRAYS_EQUALS(expected_authority.ptr, expected_authority.len, uri.authority.ptr, uri.authority.len);

    struct aws_byte_cursor expected_host = aws_byte_cursor_from_c_str("www.test.com");
    ASSERT_BIN_ARRAYS_EQUALS(expected_host.ptr, expected_host.len, uri.host_name.ptr, uri.host_name.len);

    ASSERT_UINT_EQUALS(8443, uri.port);

    struct aws_byte_cursor expected_path = aws_byte_cursor_from_c_str("/path/to/resource");
    ASSERT_BIN_ARRAYS_EQUALS(expected_path.ptr, expected_path.len, uri.path.ptr, uri.path.len);

    struct aws_byte_cursor expected_query_str =
        aws_byte_cursor_from_c_str("test1=value1&test%20space=value%20space&test2=value2&test2=value3");
    ASSERT_BIN_ARRAYS_EQUALS(
        expected_query_str.ptr, expected_query_str.len, uri.query_string.ptr, uri.query_string.len);

    struct aws_byte_cursor expected_request_uri = aws_byte_cursor_from_c_str(
        "/path/to/resource?test1=value1&test%20space=value%20space&test2=value2&test2=value3");
    ASSERT_BIN_ARRAYS_EQUALS(
        expected_request_uri.ptr, expected_request_uri.len, uri.path_and_query.ptr, uri.path_and_query.len);

    aws_uri_clean_up(&uri);
    return AWS_OP_SUCCESS;
}

AWS_TEST_CASE(uri_no_scheme_parse, s_test_uri_no_scheme_parse);

static int s_test_uri_no_port_parse(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;
    const char *str_uri =
        "https://www.test.com/path/to/resource?test1=value1&test%20space=value%20space&test2=value2&test2=value3";

    struct aws_byte_cursor uri_csr = aws_byte_cursor_from_c_str(str_uri);
    struct aws_uri uri;
    ASSERT_SUCCESS(aws_uri_init_parse(&uri, allocator, &uri_csr));

    struct aws_byte_cursor expected_scheme = aws_byte_cursor_from_c_str("https");
    ASSERT_BIN_ARRAYS_EQUALS(expected_scheme.ptr, expected_scheme.len, uri.scheme.ptr, uri.scheme.len);

    struct aws_byte_cursor expected_authority = aws_byte_cursor_from_c_str("www.test.com");
    ASSERT_BIN_ARRAYS_EQUALS(expected_authority.ptr, expected_authority.len, uri.authority.ptr, uri.authority.len);

    struct aws_byte_cursor expected_host = aws_byte_cursor_from_c_str("www.test.com");
    ASSERT_BIN_ARRAYS_EQUALS(expected_host.ptr, expected_host.len, uri.host_name.ptr, uri.host_name.len);

    ASSERT_UINT_EQUALS(0, uri.port);

    struct aws_byte_cursor expected_path = aws_byte_cursor_from_c_str("/path/to/resource");
    ASSERT_BIN_ARRAYS_EQUALS(expected_path.ptr, expected_path.len, uri.path.ptr, uri.path.len);

    struct aws_byte_cursor expected_query_str =
        aws_byte_cursor_from_c_str("test1=value1&test%20space=value%20space&test2=value2&test2=value3");
    ASSERT_BIN_ARRAYS_EQUALS(
        expected_query_str.ptr, expected_query_str.len, uri.query_string.ptr, uri.query_string.len);

    struct aws_byte_cursor expected_request_uri = aws_byte_cursor_from_c_str(
        "/path/to/resource?test1=value1&test%20space=value%20space&test2=value2&test2=value3");
    ASSERT_BIN_ARRAYS_EQUALS(
        expected_request_uri.ptr, expected_request_uri.len, uri.path_and_query.ptr, uri.path_and_query.len);

    aws_uri_clean_up(&uri);
    return AWS_OP_SUCCESS;
}

AWS_TEST_CASE(uri_no_port_parse, s_test_uri_no_port_parse);

static int s_test_uri_no_path_parse(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;
    const char *str_uri =
        "https://www.test.com:8443/?test1=value1&test%20space=value%20space&test2=value2&test2=value3";

    struct aws_byte_cursor uri_csr = aws_byte_cursor_from_c_str(str_uri);
    struct aws_uri uri;
    ASSERT_SUCCESS(aws_uri_init_parse(&uri, allocator, &uri_csr));

    struct aws_byte_cursor expected_scheme = aws_byte_cursor_from_c_str("https");
    ASSERT_BIN_ARRAYS_EQUALS(expected_scheme.ptr, expected_scheme.len, uri.scheme.ptr, uri.scheme.len);

    struct aws_byte_cursor expected_authority = aws_byte_cursor_from_c_str("www.test.com:8443");
    ASSERT_BIN_ARRAYS_EQUALS(expected_authority.ptr, expected_authority.len, uri.authority.ptr, uri.authority.len);

    struct aws_byte_cursor expected_host = aws_byte_cursor_from_c_str("www.test.com");
    ASSERT_BIN_ARRAYS_EQUALS(expected_host.ptr, expected_host.len, uri.host_name.ptr, uri.host_name.len);

    ASSERT_UINT_EQUALS(8443, uri.port);

    struct aws_byte_cursor expected_path = aws_byte_cursor_from_c_str("/");
    ASSERT_BIN_ARRAYS_EQUALS(expected_path.ptr, expected_path.len, uri.path.ptr, uri.path.len);

    struct aws_byte_cursor expected_query_str =
        aws_byte_cursor_from_c_str("test1=value1&test%20space=value%20space&test2=value2&test2=value3");
    ASSERT_BIN_ARRAYS_EQUALS(
        expected_query_str.ptr, expected_query_str.len, uri.query_string.ptr, uri.query_string.len);

    struct aws_byte_cursor expected_request_uri =
        aws_byte_cursor_from_c_str("/?test1=value1&test%20space=value%20space&test2=value2&test2=value3");
    ASSERT_BIN_ARRAYS_EQUALS(
        expected_request_uri.ptr, expected_request_uri.len, uri.path_and_query.ptr, uri.path_and_query.len);
    aws_uri_clean_up(&uri);
    return AWS_OP_SUCCESS;
}

AWS_TEST_CASE(uri_no_path_parse, s_test_uri_no_path_parse);

static int s_test_uri_no_query_parse(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;
    const char *str_uri = "https://www.test.com:8443/path/to/resource";

    struct aws_byte_cursor uri_csr = aws_byte_cursor_from_c_str(str_uri);
    struct aws_uri uri;
    ASSERT_SUCCESS(aws_uri_init_parse(&uri, allocator, &uri_csr));

    struct aws_byte_cursor expected_scheme = aws_byte_cursor_from_c_str("https");
    ASSERT_BIN_ARRAYS_EQUALS(expected_scheme.ptr, expected_scheme.len, uri.scheme.ptr, uri.scheme.len);

    struct aws_byte_cursor expected_authority = aws_byte_cursor_from_c_str("www.test.com:8443");
    ASSERT_BIN_ARRAYS_EQUALS(expected_authority.ptr, expected_authority.len, uri.authority.ptr, uri.authority.len);

    struct aws_byte_cursor expected_host = aws_byte_cursor_from_c_str("www.test.com");
    ASSERT_BIN_ARRAYS_EQUALS(expected_host.ptr, expected_host.len, uri.host_name.ptr, uri.host_name.len);

    ASSERT_UINT_EQUALS(8443, uri.port);

    struct aws_byte_cursor expected_path = aws_byte_cursor_from_c_str("/path/to/resource");
    ASSERT_BIN_ARRAYS_EQUALS(expected_path.ptr, expected_path.len, uri.path.ptr, uri.path.len);

    struct aws_byte_cursor expected_request_uri = aws_byte_cursor_from_c_str("/path/to/resource");
    ASSERT_BIN_ARRAYS_EQUALS(
        expected_request_uri.ptr, expected_request_uri.len, uri.path_and_query.ptr, uri.path_and_query.len);

    ASSERT_UINT_EQUALS(0u, uri.query_string.len);

    aws_uri_clean_up(&uri);
    return AWS_OP_SUCCESS;
}

AWS_TEST_CASE(uri_no_query_parse, s_test_uri_no_query_parse);

static int s_test_uri_minimal_parse(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;
    const char *str_uri = "www.test.com/path/to/resource";

    struct aws_byte_cursor uri_csr = aws_byte_cursor_from_c_str(str_uri);
    struct aws_uri uri;
    ASSERT_SUCCESS(aws_uri_init_parse(&uri, allocator, &uri_csr));

    ASSERT_UINT_EQUALS(0u, uri.scheme.len);

    struct aws_byte_cursor expected_authority = aws_byte_cursor_from_c_str("www.test.com");
    ASSERT_BIN_ARRAYS_EQUALS(expected_authority.ptr, expected_authority.len, uri.authority.ptr, uri.authority.len);

    struct aws_byte_cursor expected_host = aws_byte_cursor_from_c_str("www.test.com");
    ASSERT_BIN_ARRAYS_EQUALS(expected_host.ptr, expected_host.len, uri.host_name.ptr, uri.host_name.len);

    ASSERT_UINT_EQUALS(0, uri.port);

    struct aws_byte_cursor expected_path = aws_byte_cursor_from_c_str("/path/to/resource");
    ASSERT_BIN_ARRAYS_EQUALS(expected_path.ptr, expected_path.len, uri.path.ptr, uri.path.len);

    struct aws_byte_cursor expected_request_uri = aws_byte_cursor_from_c_str("/path/to/resource");
    ASSERT_BIN_ARRAYS_EQUALS(
        expected_request_uri.ptr, expected_request_uri.len, uri.path_and_query.ptr, uri.path_and_query.len);

    ASSERT_UINT_EQUALS(0u, uri.query_string.len);

    aws_uri_clean_up(&uri);
    return AWS_OP_SUCCESS;
}

AWS_TEST_CASE(uri_minimal_parse, s_test_uri_minimal_parse);

static int s_test_uri_path_and_query_only_parse(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;
    const char *str_uri = "/path/to/resource?test1=value1&test%20space=value%20space&test2=value2&test2=value3";

    struct aws_byte_cursor uri_csr = aws_byte_cursor_from_c_str(str_uri);
    struct aws_uri uri;
    ASSERT_SUCCESS(aws_uri_init_parse(&uri, allocator, &uri_csr));

    ASSERT_UINT_EQUALS(0u, uri.scheme.len);
    ASSERT_UINT_EQUALS(0u, uri.authority.len);

    struct aws_byte_cursor expected_path = aws_byte_cursor_from_c_str("/path/to/resource");
    ASSERT_BIN_ARRAYS_EQUALS(expected_path.ptr, expected_path.len, uri.path.ptr, uri.path.len);

    struct aws_byte_cursor expected_query_str =
        aws_byte_cursor_from_c_str("test1=value1&test%20space=value%20space&test2=value2&test2=value3");
    ASSERT_BIN_ARRAYS_EQUALS(
        expected_query_str.ptr, expected_query_str.len, uri.query_string.ptr, uri.query_string.len);

    struct aws_byte_cursor expected_path_and_query = aws_byte_cursor_from_c_str(
        "/path/to/resource?test1=value1&test%20space=value%20space&test2=value2&test2=value3");
    ASSERT_BIN_ARRAYS_EQUALS(
        expected_path_and_query.ptr, expected_path_and_query.len, uri.path_and_query.ptr, uri.path_and_query.len);

    aws_uri_clean_up(&uri);
    return AWS_OP_SUCCESS;
}

AWS_TEST_CASE(uri_path_and_query_only_parse, s_test_uri_path_and_query_only_parse);

static int s_test_uri_root_only_parse(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;
    const char *str_uri = "https://www.test.com";

    struct aws_byte_cursor uri_csr = aws_byte_cursor_from_c_str(str_uri);
    struct aws_uri uri;
    ASSERT_SUCCESS(aws_uri_init_parse(&uri, allocator, &uri_csr));

    struct aws_byte_cursor expected_scheme = aws_byte_cursor_from_c_str("https");
    ASSERT_BIN_ARRAYS_EQUALS(expected_scheme.ptr, expected_scheme.len, uri.scheme.ptr, uri.scheme.len);

    struct aws_byte_cursor expected_authority = aws_byte_cursor_from_c_str("www.test.com");
    ASSERT_BIN_ARRAYS_EQUALS(expected_authority.ptr, expected_authority.len, uri.authority.ptr, uri.authority.len);

    struct aws_byte_cursor expected_host = aws_byte_cursor_from_c_str("www.test.com");
    ASSERT_BIN_ARRAYS_EQUALS(expected_host.ptr, expected_host.len, uri.host_name.ptr, uri.host_name.len);

    ASSERT_UINT_EQUALS(0, uri.port);

    struct aws_byte_cursor expected_path = aws_byte_cursor_from_c_str("/");
    ASSERT_BIN_ARRAYS_EQUALS(expected_path.ptr, expected_path.len, uri.path.ptr, uri.path.len);

    ASSERT_UINT_EQUALS(0u, uri.query_string.len);

    aws_uri_clean_up(&uri);
    return AWS_OP_SUCCESS;
}

AWS_TEST_CASE(uri_root_only_parse, s_test_uri_root_only_parse);

static int s_test_uri_query_params(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;
    const char *str_uri = "https://www.test.com:8443/path/to/"
                          "resource?test1=value1&testkeyonly&test%20space=value%20space&test2=value2&test2=value3";

    struct aws_byte_cursor uri_csr = aws_byte_cursor_from_c_str(str_uri);
    struct aws_uri uri;
    ASSERT_SUCCESS(aws_uri_init_parse(&uri, allocator, &uri_csr));

    struct aws_uri_param params[5];
    AWS_ZERO_ARRAY(params);
    struct aws_array_list params_list;
    aws_array_list_init_static(&params_list, &params, 5, sizeof(struct aws_uri_param));

    ASSERT_SUCCESS(aws_uri_query_string_params(&uri, &params_list));
    ASSERT_UINT_EQUALS(5u, aws_array_list_length(&params_list));

    struct aws_byte_cursor expected_key = aws_byte_cursor_from_c_str("test1");
    struct aws_byte_cursor expected_value = aws_byte_cursor_from_c_str("value1");

    ASSERT_BIN_ARRAYS_EQUALS(expected_key.ptr, expected_key.len, params[0].key.ptr, params[0].key.len);
    ASSERT_BIN_ARRAYS_EQUALS(expected_value.ptr, expected_value.len, params[0].value.ptr, params[0].value.len);

    expected_key = aws_byte_cursor_from_c_str("testkeyonly");

    ASSERT_BIN_ARRAYS_EQUALS(expected_key.ptr, expected_key.len, params[1].key.ptr, params[1].key.len);
    ASSERT_NULL(params[1].value.ptr);
    ASSERT_UINT_EQUALS(0u, params[1].value.len);

    expected_key = aws_byte_cursor_from_c_str("test%20space");
    expected_value = aws_byte_cursor_from_c_str("value%20space");

    ASSERT_BIN_ARRAYS_EQUALS(expected_key.ptr, expected_key.len, params[2].key.ptr, params[2].key.len);
    ASSERT_BIN_ARRAYS_EQUALS(expected_value.ptr, expected_value.len, params[2].value.ptr, params[2].value.len);

    expected_key = aws_byte_cursor_from_c_str("test2");
    expected_value = aws_byte_cursor_from_c_str("value2");

    ASSERT_BIN_ARRAYS_EQUALS(expected_key.ptr, expected_key.len, params[3].key.ptr, params[3].key.len);
    ASSERT_BIN_ARRAYS_EQUALS(expected_value.ptr, expected_value.len, params[3].value.ptr, params[3].value.len);

    expected_key = aws_byte_cursor_from_c_str("test2");
    expected_value = aws_byte_cursor_from_c_str("value3");

    ASSERT_BIN_ARRAYS_EQUALS(expected_key.ptr, expected_key.len, params[4].key.ptr, params[4].key.len);
    ASSERT_BIN_ARRAYS_EQUALS(expected_value.ptr, expected_value.len, params[4].value.ptr, params[4].value.len);

    aws_uri_clean_up(&uri);
    return AWS_OP_SUCCESS;
}

AWS_TEST_CASE(uri_query_params, s_test_uri_query_params);

static int s_test_uri_invalid_scheme_parse(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;
    const char *str_uri =
        "https:/www.test.com:8443/path/to/resource?test1=value1&test%20space=value%20space&test2=value2&test2=value3";

    struct aws_byte_cursor uri_csr = aws_byte_cursor_from_c_str(str_uri);
    struct aws_uri uri;
    ASSERT_ERROR(AWS_ERROR_MALFORMED_INPUT_STRING, aws_uri_init_parse(&uri, allocator, &uri_csr));
    return AWS_OP_SUCCESS;
}

AWS_TEST_CASE(uri_invalid_scheme_parse, s_test_uri_invalid_scheme_parse);

static int s_test_uri_invalid_port_parse(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;
    const char *str_uri =
        "https://www.test.com:s8443/path/to/resource?test1=value1&test%20space=value%20space&test2=value2&test2=value3";

    struct aws_byte_cursor uri_csr = aws_byte_cursor_from_c_str(str_uri);
    struct aws_uri uri;
    ASSERT_ERROR(AWS_ERROR_MALFORMED_INPUT_STRING, aws_uri_init_parse(&uri, allocator, &uri_csr));
    return AWS_OP_SUCCESS;
}

AWS_TEST_CASE(uri_invalid_port_parse, s_test_uri_invalid_port_parse);

static int s_test_uri_port_too_large_parse(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;
    const char *str_uri = "https://www.test.com:844356/path/to/"
                          "resource?test1=value1&test%20space=value%20space&test2=value2&test2=value3";

    struct aws_byte_cursor uri_csr = aws_byte_cursor_from_c_str(str_uri);
    struct aws_uri uri;
    ASSERT_ERROR(AWS_ERROR_MALFORMED_INPUT_STRING, aws_uri_init_parse(&uri, allocator, &uri_csr));
    return AWS_OP_SUCCESS;
}

AWS_TEST_CASE(uri_port_too_large_parse, s_test_uri_port_too_large_parse);

static int s_test_uri_builder(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;
    const char *str_uri =
        "https://www.test.com:8443/path/to/resource?test1=value1&test%20space=value%20space&test2=value2&test2=value3";

    struct aws_byte_cursor uri_csr = aws_byte_cursor_from_c_str(str_uri);
    struct aws_uri uri;
    ASSERT_SUCCESS(aws_uri_init_parse(&uri, allocator, &uri_csr));

    struct aws_uri_param params[4];
    AWS_ZERO_ARRAY(params);

    struct aws_array_list params_list;
    aws_array_list_init_static(&params_list, &params, 4, sizeof(struct aws_uri_param));

    ASSERT_SUCCESS(aws_uri_query_string_params(&uri, &params_list));

    struct aws_uri_builder_options builder_args = {
        .scheme = uri.scheme,
        .path = uri.path,
        .host_name = uri.host_name,
        .port = uri.port,
        .query_params = &params_list,
    };

    struct aws_uri built_uri;
    ASSERT_SUCCESS(aws_uri_init_from_builder_options(&built_uri, allocator, &builder_args));

    struct aws_byte_cursor expected_scheme = aws_byte_cursor_from_c_str("https");
    ASSERT_BIN_ARRAYS_EQUALS(expected_scheme.ptr, expected_scheme.len, built_uri.scheme.ptr, built_uri.scheme.len);

    struct aws_byte_cursor expected_authority = aws_byte_cursor_from_c_str("www.test.com:8443");
    ASSERT_BIN_ARRAYS_EQUALS(
        expected_authority.ptr, expected_authority.len, built_uri.authority.ptr, built_uri.authority.len);

    struct aws_byte_cursor expected_host = aws_byte_cursor_from_c_str("www.test.com");
    ASSERT_BIN_ARRAYS_EQUALS(expected_host.ptr, expected_host.len, built_uri.host_name.ptr, built_uri.host_name.len);

    ASSERT_UINT_EQUALS(8443, built_uri.port);

    struct aws_byte_cursor expected_path = aws_byte_cursor_from_c_str("/path/to/resource");
    ASSERT_BIN_ARRAYS_EQUALS(expected_path.ptr, expected_path.len, built_uri.path.ptr, built_uri.path.len);

    struct aws_byte_cursor expected_query_str =
        aws_byte_cursor_from_c_str("test1=value1&test%20space=value%20space&test2=value2&test2=value3");
    ASSERT_BIN_ARRAYS_EQUALS(
        expected_query_str.ptr, expected_query_str.len, built_uri.query_string.ptr, built_uri.query_string.len);

    struct aws_byte_cursor expected_request_uri = aws_byte_cursor_from_c_str(
        "/path/to/resource?test1=value1&test%20space=value%20space&test2=value2&test2=value3");
    ASSERT_BIN_ARRAYS_EQUALS(
        expected_request_uri.ptr, expected_request_uri.len, built_uri.path_and_query.ptr, built_uri.path_and_query.len);

    aws_uri_clean_up(&uri);
    aws_uri_clean_up(&built_uri);
    return AWS_OP_SUCCESS;
}

AWS_TEST_CASE(uri_builder, s_test_uri_builder);

static int s_test_uri_builder_from_string(struct aws_allocator *allocator, void *ctx) {
    (void)ctx;
    const char *str_uri =
        "https://www.test.com:8443/path/to/resource?test1=value1&test%20space=value%20space&test2=value2&test2=value3";

    struct aws_byte_cursor uri_csr = aws_byte_cursor_from_c_str(str_uri);
    struct aws_uri uri;
    ASSERT_SUCCESS(aws_uri_init_parse(&uri, allocator, &uri_csr));

    struct aws_uri_param params[4];
    AWS_ZERO_ARRAY(params);

    struct aws_byte_cursor query_string =
        aws_byte_cursor_from_c_str("test1=value1&test%20space=value%20space&test2=value2&test2=value3");

    struct aws_uri_builder_options builder_args = {
        .scheme = uri.scheme,
        .path = uri.path,
        .host_name = uri.host_name,
        .port = uri.port,
        .query_string = query_string,
    };

    struct aws_uri built_uri;
    ASSERT_SUCCESS(aws_uri_init_from_builder_options(&built_uri, allocator, &builder_args));

    struct aws_byte_cursor expected_scheme = aws_byte_cursor_from_c_str("https");
    ASSERT_BIN_ARRAYS_EQUALS(expected_scheme.ptr, expected_scheme.len, built_uri.scheme.ptr, built_uri.scheme.len);

    struct aws_byte_cursor expected_authority = aws_byte_cursor_from_c_str("www.test.com:8443");
    ASSERT_BIN_ARRAYS_EQUALS(
        expected_authority.ptr, expected_authority.len, built_uri.authority.ptr, built_uri.authority.len);

    struct aws_byte_cursor expected_host = aws_byte_cursor_from_c_str("www.test.com");
    ASSERT_BIN_ARRAYS_EQUALS(expected_host.ptr, expected_host.len, built_uri.host_name.ptr, built_uri.host_name.len);

    ASSERT_UINT_EQUALS(8443, built_uri.port);

    struct aws_byte_cursor expected_path = aws_byte_cursor_from_c_str("/path/to/resource");
    ASSERT_BIN_ARRAYS_EQUALS(expected_path.ptr, expected_path.len, built_uri.path.ptr, built_uri.path.len);

    ASSERT_BIN_ARRAYS_EQUALS(
        query_string.ptr, query_string.len, built_uri.query_string.ptr, built_uri.query_string.len);

    struct aws_byte_cursor expected_request_uri = aws_byte_cursor_from_c_str(
        "/path/to/resource?test1=value1&test%20space=value%20space&test2=value2&test2=value3");
    ASSERT_BIN_ARRAYS_EQUALS(
        expected_request_uri.ptr, expected_request_uri.len, built_uri.path_and_query.ptr, built_uri.path_and_query.len);

    aws_uri_clean_up(&uri);
    aws_uri_clean_up(&built_uri);
    return AWS_OP_SUCCESS;
}

AWS_TEST_CASE(uri_builder_from_string, s_test_uri_builder_from_string);