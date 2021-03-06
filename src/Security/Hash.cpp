/* Copyright 2013-2019 Homegear GmbH
 *
 * libhomegear-base is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 * 
 * libhomegear-base is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with libhomegear-base.  If not, see
 * <http://www.gnu.org/licenses/>.
 * 
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU Lesser General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
*/

#include <gcrypt.h>
#include "Hash.h"

#include <vector>
#include <cstdint>

namespace BaseLib
{
namespace Security
{

template<typename Data> bool Hash::sha1(const Data& in, Data& out)
{
	out.clear();
	out.resize(gcry_md_get_algo_dlen(GCRY_MD_SHA1));
	gcry_md_hash_buffer(GCRY_MD_SHA1, out.data(), in.data(), in.size());
	return true;
}
template bool Hash::sha1<std::vector<char>>(const std::vector<char>& in, std::vector<char>& out);
template bool Hash::sha1<std::vector<uint8_t>>(const std::vector<uint8_t>& in, std::vector<uint8_t>& out);

template<typename Data> bool Hash::sha256(const Data& in, Data& out)
{
	out.clear();
	out.resize(gcry_md_get_algo_dlen(GCRY_MD_SHA256));
	gcry_md_hash_buffer(GCRY_MD_SHA256, out.data(), in.data(), in.size());
	return true;
}
template bool Hash::sha256<std::vector<char>>(const std::vector<char>& in, std::vector<char>& out);
template bool Hash::sha256<std::vector<uint8_t>>(const std::vector<uint8_t>& in, std::vector<uint8_t>& out);

template<typename Data> bool Hash::md5(const Data& in, Data& out)
{
	out.clear();
	out.resize(gcry_md_get_algo_dlen(GCRY_MD_MD5));
	gcry_md_hash_buffer(GCRY_MD_MD5, out.data(), in.data(), in.size());
	return true;
}
template bool Hash::md5<std::vector<char>>(const std::vector<char>& in, std::vector<char>& out);
template bool Hash::md5<std::vector<uint8_t>>(const std::vector<uint8_t>& in, std::vector<uint8_t>& out);

template<typename Data> bool Hash::whirlpool(const Data& in, Data& out)
{
	out.clear();
	out.resize(gcry_md_get_algo_dlen(GCRY_MD_WHIRLPOOL));
	gcry_md_hash_buffer(GCRY_MD_WHIRLPOOL, out.data(), in.data(), in.size());
	return true;
}
template bool Hash::whirlpool<std::vector<char>>(const std::vector<char>& in, std::vector<char>& out);
template bool Hash::whirlpool<std::vector<uint8_t>>(const std::vector<uint8_t>& in, std::vector<uint8_t>& out);

}
}
