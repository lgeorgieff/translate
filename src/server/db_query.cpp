// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 03/23/2015
// Description: Implements the DB_Query class that allows to query the data base for language information.
// ====================================================================================================================

// ====================================================================================================================
// This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation in version 2.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with this program; if not, write to the
// Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
// ====================================================================================================================

#include "db_query.hpp"

#include <string>
#include <sstream>

namespace lgeorgieff {
namespace translate {
namespace server {

std::string DB_Query::generate_exact_match_query(const std::string& phrase_in, const std::string& language_in,
                                                 const std::string& language_out) {
  std::stringstream ss;
  ss << "select"
        " ph_in.language as language_in,"
        " ph_in.phrase as phrase_in,"
        " wc_in.name as word_class_in,"
        " ph_in.gender as gender_in,"
        " ph_in.numerus as numerus_in,"
        " ab_in.abbreviation as abbreviation_in,"
        " co_in.comment as comment_in,"
        " ph_out.language as language_out,"
        " ph_out.phrase as phrase_out,"
        " wc_out.name as word_class_out,"
        " ph_out.gender as gender_out,"
        " ph_out.numerus as numerus_out,"
        " ab_out.abbreviation as abbriviation_out,"
        " co_out.comment as comment_out"
        "from phrase ph_in"
        " LEFT OUTER JOIN phrase_abbreviation pa_in ON ph_in.id = pa_in.phrase_id"
        " LEFT OUTER JOIN abbreviation ab_in ON ab_in.id = pa_in.abbreviation_id"
        " LEFT OUTER JOIN phrase_word_class pw_in ON ph_in.id = pw_in.phrase_id"
        " LEFT OUTER JOIN word_class wc_in ON wc_in.id = pw_in.word_class_id"
        " LEFT OUTER JOIN phrase_comment pc_in ON pc_in.phrase_id = ph_in.id"
        " LEFT OUTER JOIN comment co_in ON co_in.id = pc_in.comment_id"
        " LEFT OUTER JOIN phrase_translation pt ON pt.phrase_id_in = ph_in.id"
        " LEFT OUTER JOIN phrase ph_out ON ph_out.id = pt.phrase_id_out"
        " LEFT OUTER JOIN phrase_abbreviation pa_out ON pa_out.phrase_id = ph_out.id"
        " LEFT OUTER JOIN abbreviation ab_out ON ab_out.id = pa_out.abbreviation_id"
        " LEFT OUTER JOIN phrase_word_class pw_out ON ph_out.id = pw_out.phrase_id"
        " LEFT OUTER JOIN word_class wc_out ON wc_out.id = pw_out.word_class_id"
        " LEFT OUTER JOIN phrase_comment pc_out ON pc_out.phrase_id = ph_out.id"
        " LEFT OUTER JOIN comment co_out ON co_out.id = pc_out.comment_id"
        " where ph_in.phrase = '" << phrase_in << "' and ph_in.language = '" << language_in
     << "' and ph_out.language = '" << language_out << "';";
  return ss.str();
}

std::string DB_Query::generate_exact_match_word_class_query(const std::string& phrase_in,
                                                            const std::string& language_in,
                                                            const std::string& language_out,
                                                            const std::string& word_class) {
  std::stringstream ss;
  ss << "select"
        " ph_in.language as language_in,"
        " ph_in.phrase as phrase_in,"
        " wc_in.name as word_class_in,"
        " ph_in.gender as gender_in,"
        " ph_in.numerus as numerus_in,"
        " ab_in.abbreviation as abbreviation_in,"
        " co_in.comment as comment_in,"
        " ph_out.language as language_out,"
        " ph_out.phrase as phrase_out,"
        " wc_out.name as word_class_out,"
        " ph_out.gender as gender_out,"
        " ph_out.numerus as numerus_out,"
        " ab_out.abbreviation as abbriviation_out,"
        " co_out.comment as comment_out"
        "from phrase ph_in"
        " LEFT OUTER JOIN phrase_abbreviation pa_in ON ph_in.id = pa_in.phrase_id"
        " LEFT OUTER JOIN abbreviation ab_in ON ab_in.id = pa_in.abbreviation_id"
        " LEFT OUTER JOIN phrase_word_class pw_in ON ph_in.id = pw_in.phrase_id"
        " LEFT OUTER JOIN word_class wc_in ON wc_in.id = pw_in.word_class_id"
        " LEFT OUTER JOIN phrase_comment pc_in ON pc_in.phrase_id = ph_in.id"
        " LEFT OUTER JOIN comment co_in ON co_in.id = pc_in.comment_id"
        " LEFT OUTER JOIN phrase_translation pt ON pt.phrase_id_in = ph_in.id"
        " LEFT OUTER JOIN phrase ph_out ON ph_out.id = pt.phrase_id_out"
        " LEFT OUTER JOIN phrase_abbreviation pa_out ON pa_out.phrase_id = ph_out.id"
        " LEFT OUTER JOIN abbreviation ab_out ON ab_out.id = pa_out.abbreviation_id"
        " LEFT OUTER JOIN phrase_word_class pw_out ON ph_out.id = pw_out.phrase_id"
        " LEFT OUTER JOIN word_class wc_out ON wc_out.id = pw_out.word_class_id"
        " LEFT OUTER JOIN phrase_comment pc_out ON pc_out.phrase_id = ph_out.id"
        " LEFT OUTER JOIN comment co_out ON co_out.id = pc_out.comment_id"
        " where ph_in.phrase = '" << phrase_in << "' and ph_in.language = '" << language_in
     << "' and ph_out.language = '" << language_out << "' and wc_in.name = '" << word_class
     << "'and wc_out.name = '" << word_class << "';";
  return ss.str();
}

}  // server
}  // translate
}  // lgeorgieff
