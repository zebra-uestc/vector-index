#pragma once

#include <cinttypes>
#include <cmath>
#include <vector>

#include <immintrin.h>

// 距离定义
enum class Distance_Type : uint64_t
{
    Euclidean2,
    Inner_Product,
    Cosine_Similarity,
};

namespace euclidean2
{

float distance(const float *vector1, const float *vector2, uint64_t dimension)
{
#if defined(__AVX512F__)
    auto *vector1_pointer = vector1;
    auto *vector2_pointer = vector2;
    float __attribute__((aligned(64))) temporary_result[16];
    const float *end = vector1_pointer + ((dimension >> 4) << 4);
    __m512 difference, part_vevtor1, part_vevtor2;
    __m512 sum = _mm512_set1_ps(0);
    while (vector1_pointer < end)
    {
        part_vevtor1 = _mm512_loadu_ps(vector1_pointer);
        vector1_pointer += 16;
        part_vevtor2 = _mm512_loadu_ps(vector2_pointer);
        vector2_pointer += 16;
        difference = _mm512_sub_ps(part_vevtor1, part_vevtor2);
        sum = _mm512_add_ps(sum, _mm512_mul_ps(difference, difference));
    }
    _mm512_store_ps(temporary_result, sum);
    float distance = temporary_result[0] + temporary_result[1] + temporary_result[2] + temporary_result[3] +
                     temporary_result[4] + temporary_result[5] + temporary_result[6] + temporary_result[7] +
                     temporary_result[8] + temporary_result[9] + temporary_result[10] + temporary_result[11] +
                     temporary_result[12] + temporary_result[13] + temporary_result[14] + temporary_result[15];
    //    for (auto i = (vector1.size() >> 4) << 4; i < vector1.size(); ++i)
    //    {
    //        distance += std::pow(vector1[i] - vector2[i], 2);
    //    }
    return distance;
#elif defined(__AVX__)
    auto *vector1_pointer = vector1;
    auto *vector2_pointer = vector2;
    float __attribute__((aligned(32))) temporary_result[8];
    const float *end = vector1_pointer + ((dimension >> 4) << 4);
    __m256 difference, part_vector1, part_vector2;
    __m256 sum = _mm256_set1_ps(0);
    while (vector1_pointer < end)
    {
        part_vector1 = _mm256_loadu_ps(vector1_pointer);
        vector1_pointer += 8;
        part_vector2 = _mm256_loadu_ps(vector2_pointer);
        vector2_pointer += 8;
        difference = _mm256_sub_ps(part_vector1, part_vector2);
        sum = _mm256_add_ps(sum, _mm256_mul_ps(difference, difference));
        part_vector1 = _mm256_loadu_ps(vector1_pointer);
        vector1_pointer += 8;
        part_vector2 = _mm256_loadu_ps(vector2_pointer);
        vector2_pointer += 8;
        difference = _mm256_sub_ps(part_vector1, part_vector2);
        sum = _mm256_add_ps(sum, _mm256_mul_ps(difference, difference));
    }
    _mm256_store_ps(temporary_result, sum);
    float distance = temporary_result[0] + temporary_result[1] + temporary_result[2] + temporary_result[3] +
                     temporary_result[4] + temporary_result[5] + temporary_result[6] + temporary_result[7];
    //    for (auto i = (vector1.size() >> 4) << 4; i < vector1.size(); ++i)
    //    {
    //        distance += std::pow(vector1[i] - vector2[i], 2);
    //    }
    //    for (auto &result : temporary_result)
    //    {
    //        distance += result;
    //    }
    return distance;
#elif defined(__SSE__)
    auto *vector1_pointer = vector1;
    auto *vector2_pointer = vector2;
    float __attribute__((aligned(16))) temporary_result[4];
    const float *end = vector1_pointer + ((dimension >> 4) << 4);
    __m128 difference, part_vector1, part_vector2;
    __m128 sum = _mm128_set1_ps(0);
    while (vector1_pointer < end)
    {
        part_vector1 = _mm128_loadu_ps(vector1_pointer);
        vector1_pointer += 4;
        part_vector2 = _mm128_loadu_ps(vector2_pointer);
        vector2_pointer += 4;
        difference = _mm128_sub_ps(part_vector1, part_vector2);
        sum = _mm128_add_ps(sum, _mm128_mul_ps(difference, difference));
        part_vector1 = _mm128_loadu_ps(vector1_pointer);
        vector1_pointer += 4;
        part_vector2 = _mm128_loadu_ps(vector2_pointer);
        vector2_pointer += 4;
        difference = _mm128_sub_ps(part_vector1, part_vector2);
        sum = _mm128_add_ps(sum, _mm128_mul_ps(difference, difference));
        part_vector1 = _mm128_loadu_ps(vector1_pointer);
        vector1_pointer += 4;
        part_vector2 = _mm128_loadu_ps(vector2_pointer);
        vector2_pointer += 4;
        difference = _mm128_sub_ps(part_vector1, part_vector2);
        sum = _mm128_add_ps(sum, _mm128_mul_ps(difference, difference));
        part_vector1 = _mm128_loadu_ps(vector1_pointer);
        vector1_pointer += 4;
        part_vector2 = _mm128_loadu_ps(vector2_pointer);
        vector2_pointer += 4;
        difference = _mm128_sub_ps(part_vector1, part_vector2);
        sum = _mm128_add_ps(sum, _mm128_mul_ps(difference, difference));
    }
    _mm128_store_ps(temporary_result, sum);
    float distance = temporary_result[0] + temporary_result[1] + temporary_result[2] + temporary_result[3];
    //    for (auto i = (vector1.size() >> 4) << 4; i < vector1.size(); ++i)
    //    {
    //        distance += std::pow(vector1[i] - vector2[i], 2);
    //    }
    //    for (auto &result : temporary_result)
    //    {
    //        distance += result;
    //    }
    return distance;
#else
    float square_distance = 0;
    for (size_t i = 0; i < vector1.size(); ++i)
    {
        square_distance += std::pow(vector1[i] - vector2[i], 2);
    }
    // 根据距离定义应开方，但是不影响距离对比所以省略
    return square_distance;
#endif
}

} // namespace euclidean2

namespace inner_product
{

float distance(const std::vector<float> &vector1, const std::vector<float> &vector2)
{
    float product = 0;
    for (size_t i = 0; i < vector1.size(); ++i)
    {
        product += vector1[i] * vector2[i];
    }
    return 1 - product;
}

} // namespace inner_product

namespace cosine_similarity
{

float distance(const std::vector<float> &vector1, const std::vector<float> &vector2)
{
    float product = 0;
    float square_sum1 = 0;
    float square_sum2 = 0;
    for (size_t i = 0; i < vector1.size(); ++i)
    {
        product += vector1[i] * vector2[i];
        square_sum1 += vector1[i] * vector1[i];
        square_sum2 += vector2[i] * vector2[i];
    }
    return 1 - product / std::sqrt(square_sum1 * square_sum2);
}

} // namespace cosine_similarity

auto get_distance_calculation_function(Distance_Type distance)
{
    switch (distance)
    {
    case Distance_Type::Euclidean2:
        return euclidean2::distance;
        //    case Distance_Type::Inner_Product:
        //        return inner_product::distance;
        //    case Distance_Type::Cosine_Similarity:
        //        return cosine_similarity::distance;
    default:
        throw std::logic_error("for now, we only support 'Euclidean2', 'Inner Product', 'Cosine Similarity'. ");
    }
}
