#include "MinimizatorsAggregator.h"
#include "util/Misc.h"
#include "util/QuadMatrix.h"
#include "util/Vector.h"
#include "util/VersionedData.h"

#include <iomanip>
#include <iostream>

template <class... Args>
void println(Args && ... args)
{
    ((std::cout << args << ' '), ...) << '\n';
}

int main() {
    std::cout << std::setprecision(std::numeric_limits<double>::digits10 + 1);
    min_nd::MinimizatorsAggregator agg;
    agg.setup();

    agg.add_function(util::QuadMatrix{{{64., 19.}, {19., 32.}}}, util::Vector{{32., -20.}}, 6.);
    agg.select_function(0);
    agg.select_nd_method(0);
    auto search_res = agg.search_min_traced();

    constexpr auto printer = util::overload(
        [](const util::VdComment & comment) { println(comment.comment); },
        [](const util::VdVector & vector) { println(vector.version(), ": ", vector.vec()); },
        [](const util::VdDouble & dbl) { println(dbl.version(), ": ", dbl.value); },
        [](const auto & other) { println("not implemented for: ", static_cast<uint>(other.get_kind())); }
    );
    std::for_each(search_res.replay_data.begin(), search_res.replay_data.end(), [&printer](auto & data_part) {
        data_part->call_func(printer);
    });

    println("min point: ", search_res.min_point);
    println("min: ", search_res.min);
}
