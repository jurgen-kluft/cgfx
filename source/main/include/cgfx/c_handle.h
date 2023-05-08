#include "ccore/c_target.h"

namespace ncore
{
    namespace ngfx
    {

        template <typename T> class pool_t
        {
        public:
            pool_t()
                : m_count(0)
                , m_size(0)
                , m_alloc(nullptr)
                , m_elements(nullptr)
            {
            }

            pool_t(pool_t<T> const& other)
                : m_count(other.m_count)
                , m_size(other.m_size)
                , m_alloc(other.m_alloc)
                , m_elements(other.m_elements)
            {
            }

            static pool_t<T> sCreate(alloc_t* allocator, u32 size);

            bool        is_empty() const { return m_count == 0; }
            bool        is_valid() const { return m_count < m_size; }
            handle_t<T> obtain();
            void        release(handle_t<T> handle);

        private:
            pool_t(u32 size, T* data)
                : m_count(0)
                , m_size(size)
                , m_alloc(nullptr)
                , m_elements(data)
            {
            }

            u32      m_count;
            u32      m_size;
            alloc_t* m_alloc;
            T*       m_elements;
        };

        template <typename T> class handle_t
        {
        public:
            handle_t()
                : m_index(0)
                , m_generation(0)
            {
            }

            bool is_valid() const { return m_generation == 0; }

        private:
            handle_t(u32 index, u32 generation)
                : m_index(index)
                , m_generation(generation)
            {
            }

            u32 m_index;
            u32 m_generation;

            template <typename U> friend class pool_t;
        };

    } // namespace ngfx
} // namespace ncore