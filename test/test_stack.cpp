/******************************************************************************
The MIT License(MIT)

Embedded Template Library.
https://github.com/ETLCPP/etl
http://www.etlcpp.com

Copyright(c) 2014 jwellbelove

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files(the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions :

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
******************************************************************************/

#include "UnitTest++.h"

#include <stack>

#include "data.h"

#include "stack.h"

namespace
{
  struct Item
  {
    Item(char c_, int i_, double d_)
      : c(c_),
      i(i_),
      d(d_)
    {
    }

    char c;
    int i;
    double d;
  };

  bool operator == (const Item& lhs, const Item& rhs)
  {
    return (lhs.c == rhs.c) && (lhs.i == rhs.i) && (lhs.d == rhs.d);
  }

  SUITE(test_stack)
  {
    typedef TestDataDC<std::string>  ItemDC;
    typedef TestDataNDC<std::string> ItemNDC;

    //*************************************************************************
    TEST(test_copy_constructor)
    {
      etl::stack<int, 4> stack;

      stack.push(1);
      stack.push(2);
      stack.push(3);
      stack.push(4);

      etl::stack<int, 4> stack2(stack);

      CHECK(stack.size() == stack2.size());

      while (!stack.empty())
      {
        CHECK_EQUAL(stack.top(), stack2.top());
        stack.pop();
        stack2.pop();
      }
    }

    //*************************************************************************
    TEST(test_empty)
    {
      etl::stack<ItemNDC, 4> stack;

      CHECK(stack.empty());

      stack.push(ItemNDC("1"));

      CHECK(!stack.empty());
    }

    //*************************************************************************
    TEST(test_full)
    {
      etl::stack<int, 4> stack;

      CHECK(!stack.full());

      stack.push(1);
      stack.push(2);
      stack.push(3);
      stack.push(4);

      CHECK(stack.full());
    }

    //*************************************************************************
    TEST(test_size)
    {
      etl::stack<int, 4> stack;

      stack.push(1);
      stack.push(2);
      stack.push(3);

      CHECK_EQUAL(3U, stack.size());
    }

    //*************************************************************************
    TEST(test_max_size)
    {
      etl::stack<int, 4> stack;

      CHECK_EQUAL(4U, stack.max_size());
    }

    //*************************************************************************
    TEST(test_clear)
    {
      etl::stack<int, 4> stack;

      stack.push(1);
      stack.push(2);
      stack.clear();
      CHECK_EQUAL(0U, stack.size());
    }

    //*************************************************************************
    TEST(test_push)
    {
      etl::stack<int, 4> stack;

      stack.push(1);
      CHECK_EQUAL(1U, stack.size());

      stack.push(2);
      CHECK_EQUAL(2U, stack.size());

      CHECK_EQUAL(2, stack.top());
      stack.pop();
      CHECK_EQUAL(1, stack.top());
    }

    //*************************************************************************
    TEST(test_emplace)
    {
      etl::stack<Item, 4> stack;

      stack.emplace('a', 1, 1.2);
      CHECK_EQUAL(1U, stack.size());

      stack.emplace('b', 2, 3.4);
      CHECK_EQUAL(2U, stack.size());

      stack.emplace('c', 3, 5.6);
      CHECK_EQUAL(3U, stack.size());

      stack.emplace('d', 4, 7.8);
      CHECK_EQUAL(4U, stack.size());

      CHECK(stack.top() == Item('d', 4, 7.8));
      stack.pop();
      CHECK(stack.top() == Item('c', 3, 5.6));
      stack.pop();
      CHECK(stack.top() == Item('b', 2, 3.4));
      stack.pop();
      CHECK(stack.top() == Item('a', 1, 1.2));
    }

    //*************************************************************************
    TEST(test_push_void)
    {
      etl::stack<int, 4> stack;

      stack.push() = 1;
      CHECK_EQUAL(1U, stack.size());

      stack.push() = 2;
      CHECK_EQUAL(2U, stack.size());

      CHECK_EQUAL(2, stack.top());

      stack.pop();
      CHECK_EQUAL(1, stack.top());
    }

    //*************************************************************************
    TEST(test_push_excess)
    {
      etl::stack<int, 4> stack;

      for (size_t i = 0; i < stack.max_size(); ++i)
      {
        stack.push(1);
      }

      CHECK_THROW(stack.push(1), etl::stack_full);
    }

    //*************************************************************************
    TEST(test_pop)
    {
      etl::stack<int, 4> stack;

      stack.push(1);
      stack.push(2);
      stack.pop();
      CHECK_EQUAL(1U, stack.size());
    }

    //*************************************************************************
    TEST(test_pop_excess)
    {
      etl::stack<int, 4> stack;

      for (size_t i = 0; i < stack.max_size(); ++i)
      {
        stack.push(1);
      }

      for (size_t i = 0; i < stack.max_size(); ++i)
      {
        stack.pop();
      }

      CHECK_THROW(stack.pop(), etl::stack_empty);
    }

    //*************************************************************************
    TEST(test_top)
    {
      etl::stack<int, 4> stack;

      stack.push(1);
      stack.push(2);
      stack.push(3);

      CHECK_EQUAL(3, stack.top());

      stack.pop();
      CHECK_EQUAL(2, stack.top());

      stack.pop();
      CHECK_EQUAL(1, stack.top());
    }

    //*************************************************************************
    TEST(test_top_const)
    {
      etl::stack<int, 4> stack;
      const etl::stack<int, 4>& constQueue = stack;

      stack.push(1);
      stack.push(2);
      stack.push(3);

      CHECK_EQUAL(3, constQueue.top());

      stack.pop();
      CHECK_EQUAL(2, constQueue.top());

      stack.pop();
      CHECK_EQUAL(1, constQueue.top());
    }

    //*************************************************************************
    TEST(test_multiple_push)
    {
      etl::stack<int, 4> stack;

      stack.push(1);
      stack.push(2);
      stack.push(3);

      bool pass = true;

      if (stack.top() != 3)
      {
        pass = false;
      }

      stack.pop();

      if (stack.top() != 2)
      {
        pass = false;
      }

      stack.pop();

      if (stack.top() != 1)
      {
        pass = false;
      }

      CHECK(pass);
    }

    //*************************************************************************
    TEST(test_multiple_push_void)
    {
      etl::stack<int, 4> stack;

      stack.push() = 1;
      stack.push() = 2;
      stack.push() = 3;

      bool pass = true;

      if (stack.top() != 3)
      {
        pass = false;
      }

      stack.pop();

      if (stack.top() != 2)
      {
        pass = false;
      }

      stack.pop();

      if (stack.top() != 1)
      {
        pass = false;
      }

      CHECK(pass);
    }

    //*************************************************************************
    TEST(test_assignment)
    {
      etl::stack<int, 4> stack;

      stack.push(1);
      stack.push(2);
      stack.push(3);
      stack.push(4);

      etl::stack<int, 4> stack2;

      stack2 = stack;

      CHECK(stack.size() == stack2.size());

      while (!stack.empty())
      {
        CHECK_EQUAL(stack.top(), stack2.top());
        stack.pop();
        stack2.pop();
      }
    }

    //*************************************************************************
    TEST(test_assignment_interface)
    {
      etl::stack<int, 4> stack1;

      stack1.push(1);
      stack1.push(2);
      stack1.push(3);
      stack1.push(4);

      etl::stack<int, 4> stack2;

      etl::istack<int>& istack1 = stack1;
      etl::istack<int>& istack2 = stack2;

      istack2 = istack1;

      CHECK(istack1.size() == stack2.size());

      while (!stack1.empty())
      {
        CHECK_EQUAL(stack1.top(), stack2.top());
        stack1.pop();
        stack2.pop();
      }
    }


    //*************************************************************************
    TEST(test_self_assignment)
    {
      etl::stack<int, 4> stack;

      stack.push(1);
      stack.push(2);
      stack.push(3);
      stack.push(4);

      stack = stack;

      CHECK(stack.max_size() == stack.size());

      CHECK_EQUAL(4, stack.top());
      stack.pop();

      CHECK_EQUAL(3, stack.top());
      stack.pop();

      CHECK_EQUAL(2, stack.top());
      stack.pop();

      CHECK_EQUAL(1, stack.top());
      stack.pop();
    }

    //*************************************************************************
    TEST(test_max_n_size)
    {
      const size_t BUFFER_SIZE = 100;
      const size_t N = etl::max_stack_size<uint16_t, BUFFER_SIZE>::value;

      // Initialise the buffer.
      char buffer[BUFFER_SIZE + 4];
      std::fill(buffer, buffer + BUFFER_SIZE + 4, 0);

      // Create it.
      etl::stack<uint16_t, N>& data = etl::make_stack_at<uint16_t, N>(buffer);

      // Fill it with data.
      for (size_t i = 0; i < N; ++i)
      {
        data.push(0x5A6B);
      }

      // Check that we didn't overflow.
      CHECK_EQUAL(0, buffer[BUFFER_SIZE + 0]);
      CHECK_EQUAL(0, buffer[BUFFER_SIZE + 1]);
      CHECK_EQUAL(0, buffer[BUFFER_SIZE + 2]);
      CHECK_EQUAL(0, buffer[BUFFER_SIZE + 3]);

      // Check that we have the correct contents.
      for (size_t i = 0; i < N; ++i)
      {
        CHECK_EQUAL(0x5A6B, data.top());
        data.pop();
      }
    }
  };
}
