#pragma once

#include <cassert>
#include <vector>

namespace util {

template <class T>
struct RectangledVector
{
    template <bool is_const>
    struct Iterator;

    using iterator = Iterator<false>;
    using const_iterator = Iterator<true>;

    template <bool is_const>
    struct RowViewImpl {
        friend struct Iterator<is_const>;

        using MaybeConstOwner = std::conditional_t<is_const, const RectangledVector &, RectangledVector &>;
        using iter = std::conditional_t<is_const, typename std::vector<T>::const_iterator, typename std::vector<T>::iterator>;

        RowViewImpl() = default;
        explicit RowViewImpl(MaybeConstOwner cont, std::size_t row_num = 0)
            : m_begin(cont.m_data.begin() + row_num * cont.m_width)
            , m_end(m_begin + cont.m_width)
        {}
        RowViewImpl(iter begin, iter end)
            : m_begin(begin)
            , m_end(end)
        {}

        template <class Dummy = void, std::enable_if_t<!is_const, Dummy>>
        iter begin() noexcept { return m_begin; }
        template <class Dummy = void, std::enable_if_t<!is_const, Dummy>>
        iter end() noexcept { return m_end; }

        const iter begin() const { return m_begin; }
        const iter end() const { return m_end; }

        template <class Dummy = void, std::enable_if_t<!is_const, Dummy>>
        T & operator [] (std::size_t idx) noexcept { return *(m_begin + idx); }
        const T & operator [] (std::size_t idx) noexcept { return *(m_begin + idx); }

        iter m_begin;
        iter m_end;
    };
    using RowView = RowViewImpl<false>;
    using RowConstView = RowViewImpl<true>;

    friend struct RowViewImpl<true>;
    friend struct RowViewImpl<false>;

public:
    RectangledVector() = default;
    RectangledVector(std::vector<T> data, std::size_t width, std::size_t height)
        : m_data(std::move(data))
        , m_width(width)
        , m_height(height)
    {
        assert(m_data.size() == m_width * m_height && "RectangledVector: vector size mismatch");
    }

    RectangledVector(std::size_t width, std::size_t height)
        : m_data(width * height)
        , m_width{width}
        , m_height(height)
    {}

    iterator begin() { return iterator(*this); }
    iterator end() { return iterator(*this, m_height); }
    const_iterator begin() const { return const_iterator(*this); }
    const_iterator end() const { return const_iterator(*this, m_height); }

    RowView operator [] (std::size_t idx) noexcept { return RowView(*this, idx); }

    std::size_t rows() const noexcept { return m_height; }
    std::size_t cols() const noexcept { return m_width; }

private:
    std::vector<T> m_data;
    std::size_t m_width = 0;
    std::size_t m_height = 0;
};

template <class T>
template <bool is_const>
struct RectangledVector<T>::Iterator
{
    using MaybeConstOwner = std::conditional_t<is_const, const RectangledVector &, RectangledVector &>;

    using value_type = RowViewImpl<is_const>;
    using reference = value_type &;
    using pointer = value_type *;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::random_access_iterator_tag;

    explicit Iterator(MaybeConstOwner cont, std::size_t row_num = 0)
        : m_val(cont, row_num)
        , m_width(m_val.end() - m_val.begin())
    {}

    reference operator * () { return m_val; }
    pointer operator -> () { return &m_val; }

    Iterator & operator ++ ()
    {
        m_val.m_begin += m_width;
        m_val.m_end += m_width;
        return *this;
    }
    Iterator operator ++ (int)
    {
        Iterator prev = *this;
        operator++();
        return prev;
    }

    Iterator & operator -- ()
    {
        m_val.m_begin -= m_width;
        m_val.m_end -= m_width;
        return *this;
    }
    Iterator operator -- (int)
    {
        Iterator next = *this;
        operator--();
        return next;
    }

    Iterator & operator += (difference_type n)
    {
        n *= m_width;
        m_val.m_begin += n;
        m_val.m_end += n;
        return *this;
    }
    Iterator & operator -= (difference_type n)
    {
        n *= m_width;
        m_val.m_begin -= n;
        m_val.m_end -= n;
        return *this;
    }

    friend Iterator operator + (Iterator & it, difference_type n)
    {
        Iterator temp(it);
        temp += n;
        return temp;
    }
    friend Iterator operator + (difference_type n, Iterator & it)
    {
        Iterator temp(it);
        temp += n;
        return temp;
    }

    difference_type operator - (const Iterator & rhs) const
    {
        return (m_val.m_begin - rhs.m_val.m_begin) / m_width;
    }

    reference operator [] (difference_type n)
    {
        return *(*this + n);
    }

#define DEFINE_COMPARISON_OP(op) \
bool operator op (const Iterator & rhs) const { return m_val.m_begin op rhs.m_val.m_begin; }

DEFINE_COMPARISON_OP(==)
DEFINE_COMPARISON_OP(!=)
DEFINE_COMPARISON_OP(<)
DEFINE_COMPARISON_OP(>)
DEFINE_COMPARISON_OP(<=)
DEFINE_COMPARISON_OP(>=)
#undef DEFINE_COMPARISON_OP

private:
    RowViewImpl<is_const> m_val;
    std::size_t m_width;
};
} // namespace util
