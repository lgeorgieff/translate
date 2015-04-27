// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 04/26/2015
// Description: Implements the DbQuery class that allows to query the data base for language information.
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
#include <iostream>

namespace lgeorgieff {
namespace translate {
namespace server {

pqxx::result::const_iterator DbQuery::begin() const { return this->query_result_.begin(); }

pqxx::result::const_iterator DbQuery::end() const { return this->query_result_.end(); }

pqxx::result::const_reverse_iterator DbQuery::rbegin() const { return this->query_result_.rbegin(); }

pqxx::result::const_reverse_iterator DbQuery::rend() const { return this->query_result_.rend(); }

size_t DbQuery::size() const { return this->query_result_.size(); }

bool DbQuery::empty() const { return this->query_result_.empty(); }

void DbQuery::clear() { this->query_result_.clear(); }

DbQuery::DbQuery(const ConnectionString& connection_string) : db_connection_{nullptr} {
  this->db_connection_ = new pqxx::connection(connection_string.to_string());
  this->connection_self_created_ = true;
}

DbQuery::DbQuery(pqxx::connection* db_connection)
    : db_connection_{db_connection}, query_result_{}, connection_self_created_{false} {
  if (!this->db_connection_) throw DbException("db_connection must not be a nullptr!");
}

DbQuery::~DbQuery() {
  if (this->connection_self_created_) {
    delete this->db_connection_;
    this->db_connection_ = nullptr;
  }
}

DbQuery& DbQuery::request_phrase(const string& phrase_in, const string& language_in, const string& language_out) {
  std::string phrase_in_where_str{"is null"};
  if ("null" != phrase_in) phrase_in_where_str = "= '" + this->db_connection_->esc(phrase_in) + "'";
  std::string language_in_where_str{"is null"};
  if ("null" != language_in) language_in_where_str = "= '" + this->db_connection_->esc(language_in) + "'";
  std::string language_out_where_str{"is null"};
  if ("null" != language_out) language_out_where_str = "= '" + this->db_connection_->esc(language_out) + "'";
  std::string word_class_where_str{"is null"};
  std::stringstream ss;
  ss << "SELECT"
        " ph_in.language AS language_in,"
        " ph_in.phrase AS phrase_in,"
        " ph_in.word_clASs AS word_clASs_out,"
        " ph_in.gender AS gender_in,"
        " ph_in.numerus AS numerus_in,"
        " ab_in.abbreviation AS abbreviation_in,"
        " co_in.comment AS comment_in,"
        " ph_out.language AS language_out,"
        " ph_out.phrase AS phrase_out,"
        " ph_out.word_clASs AS word_clASs_in,"
        " ph_out.gender AS gender_out,"
        " ph_out.numerus AS numerus_out,"
        " ab_out.abbreviation AS abbriviation_out,"
        " co_out.comment AS comment_out "
        "SELECTfrom phrase ph_in"
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
        "WHERE ph_in.phrase " << phrase_in_where_str << " AND ph_in.language " << language_in_where_str
     << " AND ph_out.language " << language_out_where_str << ";";
  pqxx::work query(*this->db_connection_);
  this->query_result_ = query.exec(ss.str());
  query.commit();
  return *this;
}

DbQuery& DbQuery::request_phrase(const string& phrase_in, const string& language_in, const string& language_out,
                                   const string& word_class) {
  std::string phrase_in_where_str{"is null"};
  if ("null" != phrase_in) phrase_in_where_str = "= '" + this->db_connection_->esc(phrase_in) + "'";
  std::string language_in_where_str{"is null"};
  if ("null" != language_in) language_in_where_str = "= '" + this->db_connection_->esc(language_in) + "'";
  std::string language_out_where_str{"is null"};
  if ("null" != language_out) language_out_where_str = "= '" + this->db_connection_->esc(language_out) + "'";
  std::string word_class_where_str{"is null"};
  if ("null" != word_class) word_class_where_str = "= '" + this->db_connection_->esc(word_class) + "'";
  std::stringstream ss;
  ss << "SELECT"
        " ph_in.language AS language_in,"
        " ph_in.phrase AS phrASe_in,"
        " ph_in.word_class AS word_class_in,"
        " ph_in.gender AS gender_in,"
        " ph_in.numerus AS numerus_in,"
        " ab_in.abbreviation AS abbreviation_in,"
        " co_in.comment AS comment_in,"
        " ph_out.language AS language_out,"
        " ph_out.phrase AS phrASe_out,"
        " ph_out.word_class AS word_class_out,"
        " ph_out.gender AS gender_out,"
        " ph_out.numerus AS numerus_out,"
        " ab_out.abbreviation AS abbriviation_out,"
        " co_out.comment AS comment_out "
        "FROM phrase ph_in"
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
        "WHERE ph_in.phrase " << phrase_in_where_str << " AND ph_in.language " << language_in_where_str
     << " AND ph_out.language " << language_out_where_str << " AND ph_in.word_class " << word_class_where_str
     << " AND ph_out.word_class " << word_class_where_str << ";";
  pqxx::work query(*this->db_connection_);
  this->query_result_ = query.exec(ss.str());
  query.commit();
  return *this;
}

DbQuery& DbQuery::request_language_by_name(const string& language_name) {
  pqxx::work query(*this->db_connection_);
  std::stringstream ss;
  string language_name_where_str{"is null"};
  if ("null" != language_name) language_name_where_str = "ILIKE '" + this->db_connection_->esc(language_name) + "'";
  ss << "SELECT id FROM language WHERE name " << language_name_where_str << ";";
  this->query_result_ = query.exec(ss.str());
  query.commit();
  return *this;
}

DbQuery& DbQuery::request_language_by_id(const string& language_id) {
  pqxx::work query(*this->db_connection_);
  std::stringstream ss;
  string language_id_where_str{"is null"};
  if ("null" != language_id) language_id_where_str = "= '" + this->db_connection_->esc(language_id) + "'";
  ss << "SELECT name FROM language WHERE id " << language_id_where_str << ";";
  this->query_result_ = query.exec(ss.str());
  query.commit();
  return *this;
}

DbQuery& DbQuery::request_all_languages() {
  pqxx::work query(*this->db_connection_);
  this->query_result_ = query.exec("SELECT * FROM language;");
  query.commit();
  return *this;
}

DbQuery& DbQuery::request_word_class(const string& word_class_name) {
  pqxx::work query(*this->db_connection_);
  std::stringstream ss;
  string word_class_name_where_str{"is null"};
  if ("null" != word_class_name)
    word_class_name_where_str = "ILIKE '" + this->db_connection_->esc(word_class_name) + "'";
  ss << "SELECT id FROM word_class_description WHERE name " << word_class_name_where_str << ";";
  this->query_result_ = query.exec(ss.str());
  query.commit();
  return *this;
}

DbQuery& DbQuery::request_all_word_classes() {
  pqxx::work query(*this->db_connection_);
  this->query_result_ = query.exec("SELECT * FROM word_class_description;");
  query.commit();
  return *this;
}

DbQuery& DbQuery::request_gender(const string& gender_name) {
  pqxx::work query(*this->db_connection_);
  std::stringstream ss;
  string gender_name_where_str{"is null"};
  if ("null" != gender_name) gender_name_where_str = "ILIKE '" + this->db_connection_->esc(gender_name) + "'";
  ss << "SELECT id, description FROM gender_description WHERE name " << gender_name_where_str << ";";
  this->query_result_ = query.exec(ss.str());
  query.commit();
  return *this;
}

DbQuery& DbQuery::request_all_genders() {
  pqxx::work query(*this->db_connection_);
  this->query_result_ = query.exec("SELECT * FROM gender_description;");
  query.commit();
  return *this;
}
DbQuery& DbQuery::request_all_numeri() {
  pqxx::work query(*this->db_connection_);
  this->query_result_ = query.exec("SELECT * from unnest(enum_range(NULL::numerus));");
  query.commit();
  return *this;
}

}  // server
}  // translate
}  // lgeorgieff
