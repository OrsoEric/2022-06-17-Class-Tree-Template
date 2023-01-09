//Standard C Libraries
#include <iostream>
#include <cstdio>
#include <vector>

class My_class
{
	public:
		//Troublemaker?
		struct Node
		{
			int n_payload;
		};
		//Constructor
		My_class(void)
		{
			Node st_node;
			for (int cnt = 0; cnt < 5; cnt++)
			{
				st_node.n_payload = 33+cnt;
				gast_my_array.push_back( st_node );
				std::cout << "Push " << cnt << " | Payload: " << gast_my_array[cnt].n_payload << "\n";
			}
		}

		template <typename T>
		class iterator
		{
			public:
				//! @brief constructor for the custom iterator
				iterator(std::vector<T>& ira_parent_vector, size_t in_starting_index = 0) :
					gra_vector(ira_parent_vector),
					gu32_index(in_starting_index)
				{
					//Do nothing
					return;
				}
				iterator<T>& operator++()
				{
					gu32_index++;
					return *this;
				}
				iterator<T> operator++(int)
				{
					iterator<T> tmp(*this);
					gu32_index++;
					return tmp;
				}

				// Dereference operator (*) (returns a reference to the element at the current position)
				T &operator &(void)
				{
					return &gra_vector[gu32_index];
				}
				int get_payload( void )
				{
					return gra_vector[gu32_index].n_payload;
				}
				T get_node( void )
				{
					return gra_vector[gu32_index];
				}
				bool operator==(const iterator<T>& icl_rhs_iterator) const
				{
					return gu32_index == icl_rhs_iterator.gu32_index;
				}
				bool operator!=(const iterator<T>& icl_rhs_iterator) const
				{
					return gu32_index != icl_rhs_iterator.gu32_index;
				}

			private:
				std::vector<T>& gra_vector;
				size_t gu32_index;
		};

		iterator<Node> begin()
		{
			return iterator<Node>(gast_my_array, 0);
		}
		iterator<Node> end()
		{
			return iterator<Node>(gast_my_array, gast_my_array.size());
		}
	private:
		std::vector<Node> gast_my_array;
};

int main(void)
{
	My_class my_class_instance;

	for (My_class::iterator<My_class::Node> cl_custom_iterator=my_class_instance.begin();cl_custom_iterator!=my_class_instance.end();cl_custom_iterator++)
	{
		int n_payload = cl_custom_iterator.get_payload();
		My_class::Node st_node = cl_custom_iterator.get_node();
		//std::cout << "get_payload: " << cl_custom_iterator.get_payload();
		std::cout << "get_payload: " << n_payload << "\n";
		printf("%p | get_payload: %d | get_node n_payload: %d\n", cl_custom_iterator, n_payload, st_node.n_payload);
	}

	return 0;
}
