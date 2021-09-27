#pragma once

#include "../magic_enum/magic_enum.hpp"

#include <bitset>
#include <type_traits>

namespace Utils
{
    template <typename Enum>
    class EnumBitmask
    {
    public:
        EnumBitmask() = default;
        EnumBitmask( std::initializer_list<Enum> const & enums )
        {
            for( Enum e : enums )
            {
                m_bitset[ static_cast< utype >( e ) ] = true;
            }
        }
        ~EnumBitmask() = default;

        //Test
        bool operator | ( const Enum & val )
        {
            return m_bitset[ static_cast< utype >( val ) ] | true;
        }

        bool operator & ( const Enum & val )
        {
            return m_bitset.test( static_cast< utype >( val ) ) & true;
        }

        bool operator ^ ( const Enum & val )
        {
            return m_bitset.test( static_cast< utype >( val ) ) ^ true;
        }

       /* EnumBitmask operator | ( const EnumBitmask & val )
        {
            return (m_bitset | val.m_bitset);
        }

        EnumBitmask operator & ( const EnumBitmask & val )
        {
            return m_bitset & val.m_bitset;
        }

        EnumBitmask operator ^ ( const EnumBitmask & val )
        {
            return m_bitset ^ val.m_bitset;
        }*/

        //Assignment
        EnumBitmask & operator |= ( const Enum & val )
        {
            m_bitset[ static_cast< utype >( val ) ] = true;
            return *this;
        }

        EnumBitmask & operator &= ( const Enum & val )
        {
            bool tmp = m_bitset.test( static_cast< utype >( val ) );
            m_bitset.reset();
            m_bitset[ static_cast< utype >( val ) ] = tmp;
            return *this;
        }

        EnumBitmask & operator ^= ( const Enum & val )
        {
            bool tmp = m_bitset.test( static_cast< utype >( val ) );
            m_bitset[ static_cast< utype >( val ) ] = !tmp;
            return *this;
        }

        EnumBitmask & operator |= ( const EnumBitmask & val )
        {
            m_bitset |= val.m_bitset;
            return *this;
        }

        EnumBitmask & operator &= ( const EnumBitmask & val )
        {
            m_bitset &= val.m_bitset;
            return *this;
        }

        EnumBitmask & operator ^= ( const EnumBitmask & val )
        {
            m_bitset ^= val.m_bitset;
            return *this;
        }

        EnumBitmask & operator ~ ()
        {
            m_bitset.flip();
            return *this;
        }

        //Explicit functions which 
        void set( Enum val, bool flag )
        {
            m_bitset[ static_cast< utype >( val ) ] = flag;
        }

        bool test( size_t index ) const
        {
            return m_bitset.test( index );
        }

        size_t count() const
        {
            return m_bitset.count();
        }

    private:
        using utype = typename std::underlying_type<Enum>::type;
        std::bitset<static_cast< utype >( Enum::Total )> m_bitset;
    };
}