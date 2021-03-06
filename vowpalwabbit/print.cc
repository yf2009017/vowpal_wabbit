#include "gd.h"
#include "float.h"
#include "reductions.h"

struct print{ vw* all; };

void print_feature(vw& all, float value, float& weight)
{
  size_t index = &weight - all.reg.weight_vector;
  
  cout << index;
  if (value != 1.)
    cout << ":" << value;
  cout << " ";
}

void learn(print& p, LEARNER::base_learner& base, example& ec)
{
  label_data& ld = ec.l.simple;
  if (ld.label != FLT_MAX)
    {
      cout << ld.label << " ";
      if (ld.weight != 1 || ld.initial != 0)
	{
	  cout << ld.weight << " ";
	  if (ld.initial != 0)
	    cout << ld.initial << " ";
	}
    }
  if (ec.tag.size() > 0)
    {
      cout << '\'';
      cout.write(ec.tag.begin, ec.tag.size());
    }
  cout << "| ";
  GD::foreach_feature<vw, print_feature>(*(p.all), ec, *p.all);
  cout << endl;
}

LEARNER::base_learner* print_setup(vw& all)
{
  new_options(all, "Print options") ("print","print examples");
  if(no_new_options(all)) return NULL;
  
  print& p = calloc_or_die<print>();
  p.all = &all;
  
  size_t length = ((size_t)1) << all.num_bits;
  all.reg.weight_mask = (length << all.reg.stride_shift) - 1;
  all.reg.stride_shift = 0;
  
  LEARNER::learner<print>& ret = init_learner(&p, learn, 1);
  return make_base(ret);
}
