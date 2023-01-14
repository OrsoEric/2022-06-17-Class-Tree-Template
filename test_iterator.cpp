//Question: https://stackoverflow.com/questions/75066517/printf-and-stdcout-behave-differently-trying-to-print-out-a-structure-member-i
#include <iostream>
#include <vector>

class My_class
{
    public:
        struct Node
        {
            int n_payload;
        };
        //Constructor with example
        My_class()
        {
            Node st_node;
            for (int cnt = 0; cnt < 5; cnt++)
            {
                st_node.n_payload = 33+cnt;
                gast_my_array.push_back( st_node );
                std::cout << "Push " << cnt << " | Payload: " << gast_my_array[cnt].n_payload << "\n";
            }
        }
        //iterator
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
                //FIX: I overload the * operator to get a reference to the element of std::vector of which I can easily get the address and content
                T &operator *()
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
        //FIX: printf has undefined behaviour with %p of an object. std::cout of the address of a reference will print out the address
        std::cout << "iterator address: " << &*cl_custom_iterator << " | payload: " << (*cl_custom_iterator).n_payload << "\n";
    }

    return 0;
}
