// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 04/04/2015
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
#include "connection_string.hpp"
#include "db_exception.hpp"

#include <pqxx/pqxx>
#include <string>
#include <sstream>

namespace lgeorgieff {
namespace translate {
namespace server {

pqxx::result::const_iterator DB_Query::begin() const { return this->query_result_.begin(); }

pqxx::result::const_iterator DB_Query::end() const { return this->query_result_.end(); }

pqxx::result::const_reverse_iterator DB_Query::rbegin() const { return this->query_result_.rbegin(); }

pqxx::result::const_reverse_iterator DB_Query::rend() const { return this->query_result_.rend(); }

size_t DB_Query::size() const { return this->query_result_.empty(); }

bool DB_Query::empty() const { return this->query_result_.empty(); }

void DB_Query::clear() { this->query_result_.clear(); }

DB_Query::DB_Query(const Connection_String& connection_string) : db_connection_{nullptr} {
  this->db_connection_ = new pqxx::connection(connection_string.to_string());
  this->connection_self_created_ = true;
}

DB_Query::DB_Query(pqxx::connection* db_connection)
    : db_connection_{db_connection}, query_result_{}, connection_self_created_{false} {
  if (!this->db_connection_) throw DB_Exception("db_connection must not be a nullptr!");
}

DB_Query::~DB_Query() {
  if (this->connection_self_created_) {
    delete this->db_connection_;
    this->db_connection_ = nullptr;
  }
}

DB_Query& DB_Query::operator()(const string& phrase_in, const string& language_in, const string& language_out) {
  pqxx::work query(*this->db_connection_);
  this->query_result_ = query.exec(this->generate_exact_match_query(phrase_in, language_in, language_out));
  query.commit();
  return *this;
}

DB_Query& DB_Query::operator()(const string& phrase_in, const string& language_in, const string& language_out,
                               const string& word_class) {
  pqxx::work query(*this->db_connection_);
  this->query_result_ =
      query.exec(this->generate_exact_match_word_class_query(phrase_in, language_in, language_out, word_class));
  query.commit();
  return *this;
}

std::string DB_Query::generate_exact_match_query(const std::string& phrase_in, const std::string& language_in,
                                                 const std::string& language_out) {
  std::string phrase_in_where_str{"= '" + this->db_connection_->esc(phrase_in) + "'"};
  if ("null" == phrase_in) phrase_in_where_str = "is null";
  std::string language_in_where_str{"= '" + this->db_connection_->esc(language_in) + "'"};
  if ("null" == language_in) language_in_where_str = "is null";
  std::string language_out_where_str{"= '" + this->db_connection_->esc(language_out) + "'"};
  if ("null" == language_out) language_out_where_str = "is null";
  std::stringstream ss;
  ss << "select"
        " ph_in.language as language_in,"
        " ph_in.phrase as phrase_in,"
        " ph_in.word_class as word_class_out,"
        " ph_in.gender as gender_in,"
        " ph_in.numerus as numerus_in,"
        " ab_in.abbreviation as abbreviation_in,"
        " co_in.comment as comment_in,"
        " ph_out.language as language_out,"
        " ph_out.phrase as phrase_out,"
        " ph_out.word_class as word_class_in,"
        " ph_out.gender as gender_out,"
        " ph_out.numerus as numerus_out,"
        " ab_out.abbreviation as abbriviation_out,"
        " co_out.comment as comment_out "
        "from phrase ph_in"
        " LEFT OUTER JOIN phrase_abbreviation pa_in ON ph_in.id = pa_in.phrase_id"
        " LEFT OUTER JOIN abbreviation ab_in ON ab_in.id = pa_in.abbreviation_id"
        " LEFT OUTER JOIN phrase_comment pc_in ON pc_in.phrase_id = ph_in.id"
        " LEFT OUTER JOIN comment co_in ON co_in.id = pc_in.comment_id"
        " LEFT OUTER JOIN phrase_translation pt ON pt.phrase_id_in = ph_in.id"
        " LEFT OUTER JOIN phrase ph_out ON ph_out.id = pt.phrase_id_out"
        " LEFT OUTER JOIN phrase_abbreviation pa_out ON pa_out.phrase_id = ph_out.id"
        " LEFT OUTER JOIN abbreviation ab_out ON ab_out.id = pa_out.abbreviation_id"
        " LEFT OUTER JOIN phrase_comment pc_out ON pc_out.phrase_id = ph_out.id"
        " LEFT OUTER JOIN comment co_out ON co_out.id = pc_out.comment_id "
        "where ph_in.phrase " << phrase_in_where_str << " and ph_in.language " << language_in_where_str
     << " and ph_out.language " << language_out_where_str << ";";
  return ss.str();
}

std::string DB_Query::generate_exact_match_word_class_query(const std::string& phrase_in,
                                                            const std::string& language_in,
                                                            const std::string& language_out,
                                                            const std::string& word_class) {
  std::string phrase_in_where_str{"= '" + this->db_connection_->esc(phrase_in) + "'"};
  if ("null" == phrase_in) phrase_in_where_str = "is null";
  std::string language_in_where_str{"= '" + this->db_connection_->esc(language_in) + "'"};
  if ("null" == language_in) language_in_where_str = "is null";
  std::string language_out_where_str{"= '" + this->db_connection_->esc(language_out) + "'"};
  if ("null" == language_out) language_out_where_str = "is null";
  std::string word_class_where_str{"= '" + this->db_connection_->esc(word_class) + "'"};
  if ("null" == word_class) word_class_where_str = "is null";

  std::stringstream ss;
  ss << "select"
        " ph_in.language as language_in,"
        " ph_in.phrase as phrase_in,"
        " ph_in.word_class as word_class_in,"
        " ph_in.gender as gender_in,"
        " ph_in.numerus as numerus_in,"
        " ab_in.abbreviation as abbreviation_in,"
        " co_in.comment as comment_in,"
        " ph_out.language as language_out,"
        " ph_out.phrase as phrase_out,"
        " ph_out.word_class as word_class_out,"
        " ph_out.gender as gender_out,"
        " ph_out.numerus as numerus_out,"
        " ab_out.abbreviation as abbriviation_out,"
        " co_out.comment as comment_out "
        "from phrase ph_in"
        " LEFT OUTER JOIN phrase_abbreviation pa_in ON ph_in.id = pa_in.phrase_id"
        " LEFT OUTER JOIN abbreviation ab_in ON ab_in.id = pa_in.abbreviation_id"
        " LEFT OUTER JOIN phrase_comment pc_in ON pc_in.phrase_id = ph_in.id"
        " LEFT OUTER JOIN comment co_in ON co_in.id = pc_in.comment_id"
        " LEFT OUTER JOIN phrase_translation pt ON pt.phrase_id_in = ph_in.id"
        " LEFT OUTER JOIN phrase ph_out ON ph_out.id = pt.phrase_id_out"
        " LEFT OUTER JOIN phrase_abbreviation pa_out ON pa_out.phrase_id = ph_out.id"
        " LEFT OUTER JOIN abbreviation ab_out ON ab_out.id = pa_out.abbreviation_id"
        " LEFT OUTER JOIN phrase_comment pc_out ON pc_out.phrase_id = ph_out.id"
        " LEFT OUTER JOIN comment co_out ON co_out.id = pc_out.comment_id "
        "where ph_in.phrase " << phrase_in_where_str << " and ph_in.language " << language_in_where_str
     << " and ph_out.language " << language_out_where_str << " and ph_in.word_class " << word_class_where_str
     << " and ph_out.word_class " << word_class_where_str << ";";
  return ss.str();
}

}  // server
}  // translate
}  // lgeorgieff
