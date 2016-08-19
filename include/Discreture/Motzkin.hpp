#pragma once

#include "Combinations.hpp"
#include "DyckPaths.hpp"

namespace dscr
{	
	
	////////////////////////////////////////////////////////////////////////////////////
	/// \brief Class for iterating through all motzkin paths.
	/// \param IntType must be a SIGNED integer type.
	///
	/// Motzkin paths are paths that go from \f$(0,0)\f$ to \f$(0,2n)\f$, 
	/// which never go below the \f$ y=0\f$ line, in which each step is from \f$(x,y)\f$ to either 
	/// \f$(x+1,y+1)\f$ or \f$(x+1,y-1)\f$ or \f$(x+1,y)\f$
	/// #Example Usage:
	///
	///		motzkin_paths X(4)
	///		for (const auto& x : X)
	///			cout << x << endl;
	/// Prints out:
	///		[ 0 0 0 0 ]
	///		[ 1 -1 0 0 ]
	///		[ 1 0 -1 0 ]
	///		[ 0 1 -1 0 ]
	///		[ 1 0 0 -1 ]
	///		[ 0 1 0 -1 ]
	///		[ 0 0 1 -1 ]
	///		[ 1 1 -1 -1 ]
	///		[ 1 -1 1 -1 ]
	///
	///
	/// # Example: Parenthesis
	///
	/// 	motzkin_paths X(4)
	/// 	for (const auto& x : X)
	/// 		cout << motzkin_paths::to_string(x, "(-)") << endl;
	///
	/// Prints out:
	///		----
	///		()--
	///		(-)-
	///		-()-
	///		(--)
	///		-(-)
	///		--()
	///		(())
	///		()()
	///
	/////////////////////////////////////////////////////////////////////////////////////

	template <class IntType>
	class basic_motzkin_paths
	{
	public:
		
		typedef long long int difference_type;
		typedef unsigned long long int size_type;
		typedef vector<IntType> value_type;
		typedef vector<IntType> motzkin_path;
		typedef typename basic_combinations<IntType>::iterator comb_i;
		typedef typename basic_dyck_paths<IntType>::iterator dyck_i;
		
		static std::string to_string(const motzkin_path& data, const string& delim = "(-)")
		{
			std::string toReturn;
			for (auto i : data)
			{
				auto j = 1-i;
				toReturn.push_back(delim[j]);
			}
			return toReturn;
		}
		
		// **************** End static functions
		
	public:
		
		////////////////////////////////////////////////////////////
		/// \brief Constructor
		///
		/// \param n is an integer >= 0
		///
		////////////////////////////////////////////////////////////
		basic_motzkin_paths(IntType n) : m_n(n), m_begin(n), m_end()
		{
			m_end.m_ID = size();
		}
		
		////////////////////////////////////////////////////////////
		/// \brief The total number of motzkin_paths
		///
		/// \return M_n
		///
		////////////////////////////////////////////////////////////
		size_type size() const { return motzkin(m_n); }
		
		
		IntType get_n() const { return m_n; }
		
		
		////////////////////////////////////////////////////////////
		/// \brief Forward iterator class.
		////////////////////////////////////////////////////////////
		class iterator : public std::iterator<std::forward_iterator_tag,motzkin_path>
		{
		public:
			iterator() : m_ID(0), m_data(), m_comb(), m_dyck(), m_numnonzerohalved(0) {} //empty initializer
		public:
			explicit iterator(IntType n) : m_ID(0), m_data(n,0), m_comb(n,0), m_dyck(0), m_numnonzerohalved(0)
			{
			}
			
			//prefix
			inline iterator& operator++()
			{
				++m_ID;
				auto n = m_data.size();
				if (m_ID == motzkin(n))
					return *this;
				
				++m_comb;
				if (m_comb.is_at_end(n))
				{
// 					cout << "Combination at end!" << endl;
					//Reset combinations and make a new dyck path.
					++m_dyck;
// 					cout << "added 1 to dyck" << endl;
					if (m_dyck.is_at_end(m_numnonzerohalved))
					{
	// 					cout << "dyck at end!" << endl;
						m_numnonzerohalved += 1;
						
// 						cout << "Resetting dyck..." << endl;
						m_dyck.reset(m_numnonzerohalved);
					}
// 					cout << "Resetting combination..." << endl;
					m_comb.reset(n,2*m_numnonzerohalved);
					
				}
// 				cout << "Done! Converting!" << endl;
				ConvertToMotzkin();
				return *this;
			}
			
			inline iterator& operator--()
			{
				
				if (m_ID == 0)
					return *this;
				
				--m_ID;
				
				//TODO: Not implemented yet!
				
				return *this;
			}
			
			inline const vector<IntType>& operator*() const
			{
				return m_data;
			}
			
			inline const motzkin_path* operator->() const
			{ 
				return & operator*();
			}
			
			friend difference_type operator-(const iterator& lhs, const iterator& rhs)
			{
				return static_cast<difference_type>(lhs.ID()) - rhs.ID();
			}
			
			inline size_type ID() const { return m_ID; }
			
			inline bool operator==(const iterator& it) const
			{
				return it.ID() == ID();
			}
			
			inline bool operator!=(const iterator& it) const
			{
				return it.ID() != ID();
			}
			
			void reset(IntType n)
			{
				m_ID = 0;
				m_data.resize(2*n);
				for (size_t i = 0; i < n; ++i)
					m_data[i] = 1;
				for (size_t i = n; i < 2*n; ++i)
					m_data[i] = -1;
				m_comb.reset(0);
				m_dyck.reset(n);
			}
			
		private:
			size_type m_ID;
			motzkin_path m_data;
			comb_i m_comb;
			dyck_i m_dyck;
			IntType m_numnonzerohalved;
			
			void ConvertToMotzkin()
			{
// 				cout << "Converting: " << *m_comb << " and " << *m_dyck << endl;
				for(size_t i = 0; i < m_data.size(); ++i)
				{
					m_data[i] = 0;
				}
				size_t count = 0;
				for(auto x : (*m_comb))
				{
					m_data[x] = (*m_dyck)[count];
					++count;
				}
			}
			
			friend class basic_motzkin_paths;
		}; // end class iterator

		const iterator& begin() const
		{
			return m_begin;
		}
		
		const iterator& end() const
		{
			return m_end;
		}

	private:
		IntType m_n;
		iterator m_begin;	
		iterator m_end;
		
	}; // end class basic_motzkin_paths
	
	using motzkin_paths = basic_motzkin_paths<int>;

} // end namespace dscr;