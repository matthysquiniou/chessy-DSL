#include "VM_Table.h"
#include <immintrin.h>

// Add operand
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateAddLine(std::index_sequence<jSize...>)
{
    return array<OperationPtr, sizeof...(jSize)>{
        &Add<static_cast<ConstantType>(iSize), static_cast<ConstantType>(jSize)>...
    };
}

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateAddTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js)
{
    return array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)>{
        CreateAddLine<iSize>(js)...
    };
}


// Sub operand
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateSubLine(std::index_sequence<jSize...>)
{
    return array<OperationPtr, sizeof...(jSize)>{
        &Sub<static_cast<ConstantType>(iSize), static_cast<ConstantType>(jSize)>...
    };
}

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateSubTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js)
{
    return array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)>{
        CreateSubLine<iSize>(js)...
    };
}


// Mul type
template <size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateMulLine(std::index_sequence<jSize...>)
{
    return array<OperationPtr, sizeof...(jSize)>{
        &Mul<static_cast<ConstantType>(iSize), static_cast<ConstantType>(jSize)>...
    };
}

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateMulTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js)
{
    return array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)>{
        CreateMulLine<iSize>(js)...
    };
}


// Div type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateDivLine(std::index_sequence<jSize...>)
{
    return array<OperationPtr, sizeof...(jSize)>{
        &Div<static_cast<ConstantType>(iSize), static_cast<ConstantType>(jSize)>...
    };
}

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateDivTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js)
{
    return array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)>{
        CreateDivLine<iSize>(js)...
    };
}


// Mod type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateModLine(std::index_sequence<jSize...>)
{
    return array<OperationPtr, sizeof...(jSize)>{
        &Mod<static_cast<ConstantType>(iSize), static_cast<ConstantType>(jSize)>...
    };
}

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateModTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js)
{
    return array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)>{
        CreateModLine<iSize>(js)...
    };
}


// Pow type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreatePowLine(std::index_sequence<jSize...>)
{
    return array<OperationPtr, sizeof...(jSize)>{
        &Pow<static_cast<ConstantType>(iSize), static_cast<ConstantType>(jSize)>...
    };
}

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreatePowTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js)
{
    return array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)>{
        CreatePowLine<iSize>(js)...
    };
}


// Neg type
template<size... iSize>
constexpr array<OperationPtrSolo, sizeof...(iSize)> CreateNegTable(std::index_sequence<iSize...>)
{
    return array<OperationPtrSolo, sizeof...(iSize)>{
        &Neg<static_cast<ConstantType>(iSize)>...
    };
}



// And type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateAndLine(std::index_sequence<jSize...>)
{
    return array<OperationPtr, sizeof...(jSize)>{
        &And<static_cast<ConstantType>(iSize), static_cast<ConstantType>(jSize)>...
    };
}

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateAndTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js)
{
    return array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)>{
        CreateAndLine<iSize>(js)...
    };
}


// Or type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateOrLine(std::index_sequence<jSize...>)
{
    return array<OperationPtr, sizeof...(jSize)>{
        &Or<static_cast<ConstantType>(iSize), static_cast<ConstantType>(jSize)>...
    };
}

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateOrTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js)
{
    return array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)>{
        CreateOrLine<iSize>(js)...
    };
}


// Xor type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateXorLine(std::index_sequence<jSize...>)
{
    return array<OperationPtr, sizeof...(jSize)>{
        &Xor<static_cast<ConstantType>(iSize), static_cast<ConstantType>(jSize)>...
    };
}

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateXorTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js)
{
    return array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)>{
        CreateXorLine<iSize>(js)...
    };
}


// Not type
template<size... iSize>
constexpr array<OperationPtrSolo, sizeof...(iSize)> CreateNotTable(std::index_sequence<iSize...>)
{
    return array<OperationPtrSolo, sizeof...(iSize)>{
        &Not<static_cast<ConstantType>(iSize)>...
    };
}


// Shl type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateShlLine(std::index_sequence<jSize...>)
{
    return array<OperationPtr, sizeof...(jSize)>{
        &Shl<static_cast<ConstantType>(iSize), static_cast<ConstantType>(jSize)>...
    };
}

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateShlTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js)
{
    return array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)>{
        CreateShlLine<iSize>(js)...
    };
}


// Shr type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateShrLine(std::index_sequence<jSize...>)
{
    return array<OperationPtr, sizeof...(jSize)>{
        &Shr<static_cast<ConstantType>(iSize), static_cast<ConstantType>(jSize)>...
    };
}

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateShrTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js)
{
    return array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)>{
        CreateShrLine<iSize>(js)...
    };
}


// Is Equal type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateCmpEqLine(std::index_sequence<jSize...>)
{
    return array<OperationPtr, sizeof...(jSize)>{
        &CmpEq<static_cast<ConstantType>(iSize), static_cast<ConstantType>(jSize)>...
    };
}

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateCmpEqTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js)
{
    return array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)>{
        CreateCmpEqLine<iSize>(js)...
    };
}


// Is Different type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateCmpNeLine(std::index_sequence<jSize...>)
{
    return array<OperationPtr, sizeof...(jSize)>{
        &CmpNe<static_cast<ConstantType>(iSize), static_cast<ConstantType>(jSize)>...
    };
}

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateCmpNeTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js)
{
    return array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)>{
        CreateCmpNeLine<iSize>(js)...
    };
}


// Lesser Than type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateCmpLtLine(std::index_sequence<jSize...>)
{
    return array<OperationPtr, sizeof...(jSize)>{
        &CmpLt<static_cast<ConstantType>(iSize), static_cast<ConstantType>(jSize)>...
    };
}

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateCmpLtTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js)
{
    return array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)>{
        CreateCmpLtLine<iSize>(js)...
    };
}


// Lesser or Equal type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateCmpLeLine(std::index_sequence<jSize...>)
{
    return array<OperationPtr, sizeof...(jSize)>{
        &CmpLe<static_cast<ConstantType>(iSize), static_cast<ConstantType>(jSize)>...
    };
}

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateCmpLeTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js)
{
    return array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)>{
        CreateCmpLeLine<iSize>(js)...
    };
}


// Greater Than type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateCmpGtLine(std::index_sequence<jSize...>)
{
    return array<OperationPtr, sizeof...(jSize)>{
        &CmpGt<static_cast<ConstantType>(iSize), static_cast<ConstantType>(jSize)>...
    };
}

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateCmpGtTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js)
{
    return array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)>{
        CreateCmpGtLine<iSize>(js)...
    };
}


// Greater or Equal type
template<size iSize, size... jSize>
constexpr array<OperationPtr, sizeof...(jSize)> CreateCmpGeLine(std::index_sequence<jSize...>)
{
    return array<OperationPtr, sizeof...(jSize)>{
        &CmpGe<static_cast<ConstantType>(iSize), static_cast<ConstantType>(jSize)>...
    };
}

template<size... iSize, size... jSize>
constexpr array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)> CreateCmpGeTable(std::index_sequence<iSize...>, std::index_sequence<jSize...> js)
{
    return array<array<OperationPtr, sizeof...(jSize)>, sizeof...(iSize)>{
        CreateCmpGeLine<iSize>(js)...
    };
}

inline VectorI32x8 operator+(const VectorI32x8& a, const VectorI32x8& b) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b.lanes));
    __m256i vr = _mm256_add_epi32(va, vb);
    VectorI32x8 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI32x8 operator-(const VectorI32x8& a, const VectorI32x8& b) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b.lanes));
    __m256i vr = _mm256_sub_epi32(va, vb);
    VectorI32x8 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI32x8 operator*(const VectorI32x8& a, const VectorI32x8& b) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b.lanes));
    __m256i vr = _mm256_mullo_epi32(va, vb);
    VectorI32x8 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI32x8 operator&(const VectorI32x8& a, const VectorI32x8& b) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b.lanes));
    __m256i vr = _mm256_and_si256(va, vb);
    VectorI32x8 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI32x8 operator|(const VectorI32x8& a, const VectorI32x8& b) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b.lanes));
    __m256i vr = _mm256_or_si256(va, vb);
    VectorI32x8 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI32x8 operator^(const VectorI32x8& a, const VectorI32x8& b) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b.lanes));
    __m256i vr = _mm256_xor_si256(va, vb);
    VectorI32x8 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI32x8 operator~(const VectorI32x8& a) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vr = _mm256_xor_si256(va, _mm256_set1_epi32(-1));
    VectorI32x8 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI32x8 operator==(const VectorI32x8& a, const VectorI32x8& b) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b.lanes));
    __m256i vr = _mm256_cmpeq_epi32(va, vb);
    VectorI32x8 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI32x8 operator!=(const VectorI32x8& a, const VectorI32x8& b) {
    return ~(a == b);
}
inline VectorI32x8 operator<(const VectorI32x8& a, const VectorI32x8& b) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b.lanes));
    __m256i vr = _mm256_cmpgt_epi32(vb, va);
    VectorI32x8 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI32x8 operator>(const VectorI32x8& a, const VectorI32x8& b) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b.lanes));
    __m256i vr = _mm256_cmpgt_epi32(va, vb);
    VectorI32x8 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI32x8 operator<=(const VectorI32x8& a, const VectorI32x8& b) {
    return ~(a > b);
}
inline VectorI32x8 operator>=(const VectorI32x8& a, const VectorI32x8& b) {
    return ~(a < b);
}

inline VectorI64x4 operator+(const VectorI64x4& a, const VectorI64x4& b) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b.lanes));
    __m256i vr = _mm256_add_epi64(va, vb);
    VectorI64x4 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI64x4 operator-(const VectorI64x4& a, const VectorI64x4& b) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b.lanes));
    __m256i vr = _mm256_sub_epi64(va, vb);
    VectorI64x4 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI64x4 operator*(const VectorI64x4& a, const VectorI64x4& b) {
    VectorI64x4 r;
    for (int i = 0; i < 4; ++i) r.lanes[i] = a.lanes[i] * b.lanes[i];
    return r;
}
inline VectorI64x4 operator&(const VectorI64x4& a, const VectorI64x4& b) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b.lanes));
    __m256i vr = _mm256_and_si256(va, vb);
    VectorI64x4 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI64x4 operator|(const VectorI64x4& a, const VectorI64x4& b) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b.lanes));
    __m256i vr = _mm256_or_si256(va, vb);
    VectorI64x4 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI64x4 operator^(const VectorI64x4& a, const VectorI64x4& b) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b.lanes));
    __m256i vr = _mm256_xor_si256(va, vb);
    VectorI64x4 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI64x4 operator~(const VectorI64x4& a) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vr = _mm256_xor_si256(va, _mm256_set1_epi64x(-1));
    VectorI64x4 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI64x4 operator==(const VectorI64x4& a, const VectorI64x4& b) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b.lanes));
    __m256i vr = _mm256_cmpeq_epi64(va, vb);
    VectorI64x4 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI64x4 operator!=(const VectorI64x4& a, const VectorI64x4& b) {
    return ~(a == b);
}
inline VectorI64x4 operator<(const VectorI64x4& a, const VectorI64x4& b) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b.lanes));
    __m256i vr = _mm256_cmpgt_epi64(vb, va);
    VectorI64x4 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI64x4 operator>(const VectorI64x4& a, const VectorI64x4& b) {
    __m256i va = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(a.lanes));
    __m256i vb = _mm256_loadu_si256(reinterpret_cast<const __m256i*>(b.lanes));
    __m256i vr = _mm256_cmpgt_epi64(va, vb);
    VectorI64x4 r; _mm256_storeu_si256(reinterpret_cast<__m256i*>(r.lanes), vr); return r;
}
inline VectorI64x4 operator<=(const VectorI64x4& a, const VectorI64x4& b) {
    return ~(a > b);
}
inline VectorI64x4 operator>=(const VectorI64x4& a, const VectorI64x4& b) {
    return ~(a < b);
}