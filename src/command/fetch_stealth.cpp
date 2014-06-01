/*
 * Copyright (c) 2011-2014 sx developers (see AUTHORS)
 *
 * This file is part of sx.
 *
 * sx is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#include <bitcoin/bitcoin.hpp>
#include <obelisk/obelisk.hpp>
#include <sx/command/fetch_stealth.hpp>
#include <sx/config.hpp>
#include <sx/utility/client.hpp>
#include <sx/utility/console.hpp>

using namespace bc;
using std::placeholders::_1;
using std::placeholders::_2;

// TODO: this should be a member of sx::extensions::fetch_stealth,
// otherwise concurrent test execution will collide on shared state.
bool fetch_stealth_stopped = false;

// TODO: fetch_stealth_stopped should be passed here via closure
// or by converting this to a member function.
void stealth_fetched(const std::error_code& ec,
    const blockchain::stealth_list& stealth_results)
{
    fetch_stealth_stopped = true;

    if (ec)
    {
        log_error() << "Stealth fetch failed: " << ec.message();
        return;
    }

    for (const blockchain::stealth_row& row: stealth_results)
        std::cout << "ephemkey: " << row.ephemkey
            << " address: " << row.address.encoded()
            << " tx_hash: " << row.transaction_hash << std::endl;
}

bool sx::extensions::fetch_stealth::invoke(const int argc, const char* argv[])
{
    if (!validate_argument_range(argc, example(), 1, 3))
        return false;

    stealth_prefix prefix(argc > 1 ? std::string(argv[1]) : "");
    size_t height = (argc == 3 ? boost::lexical_cast<size_t>(argv[2]) : 0);

    OBELISK_FULLNODE(pool, fullnode);
    fullnode.blockchain.fetch_stealth(prefix, 
        std::bind(stealth_fetched, _1, _2), height);
    poll(fullnode, pool, fetch_stealth_stopped);

    return true;
}