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
#include <iostream>
#include <bitcoin/bitcoin.hpp>
#include <wallet/wallet.hpp>
#include <sx/command/mpk.hpp>
#include <sx/utility/console.hpp>

using namespace bc;
using namespace libwallet;

bool sx::extensions::mpk::invoke(const int argc, const char* argv[])
{
    if (!validate_argument_range(argc, example(), 1, 1))
        return false;

    deterministic_wallet wallet;
    std::string seed = sx::read_stdin();

    if (!wallet.set_seed(seed))
    {
        std::cerr << "mpk: Error setting seed" << std::endl;
        return false;
    }

    const data_chunk mpk = wallet.master_public_key();
    std::cout << mpk << std::endl;
    return true;
}