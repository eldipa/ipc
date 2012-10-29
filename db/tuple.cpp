
#include <sqlite3.h>
#include <unistd.h>
#include "valueerror.h"
#include "dberror.h"
#include "tuple.h"
#include "log.h"


void TupleIterator::check(int i) const {
   if(i < 0 or i >= size())
      throw ValueError("Out of range [%i,%i). Index used was %i.", 0, size(), i);
}

int TupleIterator::size() const {
   return sqlite3_column_count(&stmt);
}


TupleIterator::TupleIterator(sqlite3 &db, sqlite3_stmt &stmt, bool done) : db(db), stmt(stmt), done(done), reset(not done) {
   if(not done) {
      ++(*this);  //Set the iterator to point the first element (if any)
   }
}

bool TupleIterator::operator!=(const TupleIterator& t) {
   return this->done != t.done;
}

TupleIterator& TupleIterator::operator++() {
   while(true) {
      switch(sqlite3_step(&stmt)) {
         case SQLITE_DONE:
            done = true;
         case SQLITE_ROW:
            return *this;
         case SQLITE_BUSY:
            sleep(1);
            break;
         default:
            throw DBError(db, "An error ocurred when executing the statement.");
      }
   }
}


TupleIterator::~TupleIterator() throw() try {
   if(reset)
      if(sqlite3_reset(&stmt) != SQLITE_OK)
         throw DBError(db, "The statement cannot be reset (or initializated).");
} catch(const std::exception &e) {
   Log::crit("An exception happend during the course of a destructor:\n%s", e.what());
} catch(...) {
   Log::crit("An unknow exception happend during the course of a destructor.");
}

