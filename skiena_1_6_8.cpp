// compile: g++ -std=c++11 skiena_1_6_7.cpp (to enable std::to_string())
#include <iostream>
#include <vector>
#include <string>
#include <boost/tokenizer.hpp>
using namespace std;
using namespace boost;

// this class represents a list of integer indices. its main purpose is for the
// convenience of printing via operator string()
class Indices
{
public:
  vector<int> list;

  operator string()
  {
    string builder;
    for (vector<int>::iterator it = list.begin(); it != list.end(); it++)
      builder.append(" ").append(to_string(*it+1));
    return builder;
  }
};

// this class represents a ballot, which is a vector of int votes
class Ballot
{
public:
  vector<int> votes;

  // this constructor initializes the ballot with integers (votes) extracted
  // from a string
  Ballot(string& line)
  {
    tokenizer<char_separator<char> > tokens(line);
    for (tokenizer<char_separator<char> >::iterator it = tokens.begin(); it != tokens.end(); it++)
    {
      int vote = stoi(*it);
      votes.push_back(vote);
    }
  }

  // class Ballot
  operator string()
  {
    string builder;
    builder.append("[");
    for (vector<int>::iterator it = votes.begin(); it != votes.end(); it++)
      builder.append(to_string(*it)).append(" ");
    builder.pop_back();
    builder.append("]");
    builder.append("\n");
    return builder;
  }

  static Indices indices;
  // unary function (that accepts an element of vector<int> as argument and
  // returns bool) as required by std::remove_if(). this function must be static
  // so std::remove_if() can call it. this function returns whether an int item
  // is found in a vector<int> (indices)
  // class Ballot
  static bool find_loser(int item)
  {
    bool found = (find(indices.list.begin(), indices.list.end(), item-1) != indices.list.end());
    return found;
  }

  // this method removes all votes that match the list of eliminated candidates
  // class Ballot
  void remove_votes(Indices& losers_indices)
  {
    // hack: save a reference to losers_indices so it can be used/referenced
    // inside the predicate find_loser(), which is static, so this reference
    // must be static as well.
    indices = losers_indices;
    // use the remove-erase idiom to remove an element from vector while
    // iterating over it.
    votes.erase(remove_if(votes.begin(), votes.end(), find_loser), votes.end());
  }
};
Indices Ballot::indices;

// this class is comprised of vector<Ballots*> and not vector<Ballot>. this
// class serves as a collection of pointers to a subset of ballots within the
// master list of ballots. this class is used by class Candidate and by output()
// in order to display/remove votes from individual ballots without removing
// the ballots from the master list. this class is born out of necessity, since
// C++ distinguishes between an object and its pointer, whereas Java doesn't and
// which spares the creation of this class.
class BallotPointers
{
public:
  vector<Ballot*> list;

  operator string()
  {
    string builder;
    for (vector<Ballot*>::iterator it = list.begin(); it != list.end(); it++)
      builder.append(" ").append(**it);
    return builder;
  }

  // this method is similar in functionality to Ballots::match()
  // class BallotPointers
  BallotPointers match(int vote)
  {
    BallotPointers result;
    for (vector<Ballot*>::iterator it = list.begin(); it != list.end(); it++)
    {
      if ((*it)->votes[0] == vote)
        // retain the address (within vector<Ballot>) of the matching ballot
        result.list.push_back(*it);
    }
    return result;
  }
};

// this class represents the master list of all ballots of the current poll.
// each ballot is comprised of many int votes. the votes may be removed from
// ballots, but no ballots may be removed from this master list.
class Ballots
{
public:
  vector<Ballot> list;

  operator string()
  {
    string builder;
    for (vector<Ballot>::iterator it = list.begin(); it != list.end(); it++)
      builder.append(" ").append(*it);
    return builder;
  }

  // class Ballots
  void remove_votes(Indices& losers_indices)
  {
    for (vector<Ballot>::iterator it = list.begin(); it != list.end(); it++)
      it->remove_votes(losers_indices);
  }

  // this method searches through a master list of ballots for those whose first
  // vote match the current candidate and returns a list of pointers to matching
  // ballots. the master list of ballots remains unmodified.
  // class Ballots
  BallotPointers match(int candidate)
  {
    BallotPointers result;
    for (vector<Ballot>::iterator it = list.begin(); it != list.end(); it++)
    {
      if (it->votes[0] == candidate)
        // retain the address (within vector<Ballot>) of the matching ballot
        result.list.push_back(&*it);
    }
    return result;
  }
};

// this class represents a candidate, which comprises of a name and a
// vector<Ballot*>, which allows individual candidates to modify the contents of
// Ballot's residing in the master list of ballots.
class Candidate
{
public:
  string name;
  BallotPointers ballots;

  Candidate(string& nom)
  {
    name = nom;
  }

  int size()
  {
    return ballots.list.size();
  }

  // this method adds new ballot pointers to the current vector<Ballot*> of this
  // candidate
  // class Candidate
  void add_ballots(BallotPointers new_ballots)
  {
    if (ballots.list.size() != 0 && new_ballots.list.size() != 0)
    {
      for (vector<Ballot*>::iterator it = new_ballots.list.begin(); it != new_ballots.list.end(); it++)
        ballots.list.push_back(*it);
    }
  }

  // class Candidate
  operator string()
  {
    string builder;
    builder.append("<").append(name).append("> ");
    builder.append(to_string(ballots.list.size())).append(" ballots\n");
    builder.append(ballots);
    return builder;
  }
};

// this class comprises of a vector<Candidate*>. it's created via Candidates::
// min_vote_candidates(), which is a subset of all candidates. it serves to
// identify the losing candidates as well as their voting numbers (indices), and
// to collect the ballots associate with these candidates so that their votes
// can be removed and re-counted.
class CandidatePointers
{
public:
  vector<Candidate*> list;
  Candidate* base;

  CandidatePointers(Candidate* address)
  {
    base = address;
  }

  // this method returns a list of indices associated with these candidates. the
  // indices are also the candidates' voting numbers/rankings.
  // class CandidatePointers
  Indices indices()
  {
    Indices result;
    for (vector<Candidate*>::iterator it = list.begin(); it != list.end(); it++)
      result.list.push_back(*it - base);
    return result;
  }

  // class CandidatePointers
  operator string()
  {
    string builder;
    for (vector<Candidate*>::iterator it = list.begin(); it != list.end(); it++)
      builder.append(**it);
    return builder;
  }

  // this method returns a list of all ballots of all candidates in this object
  // class CandidatePointers
  BallotPointers all_ballots()
  {
    BallotPointers result;
    for (vector<Candidate*>::iterator it = list.begin(); it != list.end(); it++)
      for (vector<Ballot*>::iterator jt = (*it)->ballots.list.begin(); jt != (*it)->ballots.list.end(); jt++)
        result.list.push_back(*jt);

    return result;
  }

  // this method removes all ballots for each candidate in the list of candidates
  // class CandidatePointers
  void eliminate()
  {
    for (vector<Candidate*>::iterator it = list.begin(); it != list.end(); it++)
      (*it)->ballots.list.clear();
  }
};

// this class represents a list of all candidates
class Candidates
{
public:
  vector<Candidate> list;

  operator string()
  {
    string builder;
    for (vector<Candidate>::iterator it = list.begin(); it != list.end(); it++)
      builder.append(*it);
    return builder;
  }

  // this method finds a candidate whose total ballot count is more than 50% of
  // the total ballot count
  // class Candidates
  Candidate* find_winner(int ballot_count)
  {
    for (int i = 0; i < list.size(); i++)
    {
      Candidate* candidate = &list[i];
      // cout << "total ballots: " << ballot_count << ", candidate: " << (i+1) << ", ballot count: " << candidate->ballots.list.size() << endl;
      // look for candidate with ballot count more than 50% of the total votes
      if (candidate->ballots.list.size() / (float)ballot_count >= .5)
        return candidate;
    }
    return NULL;
  }

  // this method returns the lowest number of votes among all candidates
  // class Candidates
  int min_vote_count()
  {
    int min_vote_count = numeric_limits<int>::max();
    for (vector<Candidate>::iterator it = list.begin(); it != list.end(); it++)
    {
      int vote_count = it->ballots.list.size();
      if (vote_count != 0)
        if (vote_count < min_vote_count)
          min_vote_count = vote_count;
    }

    // cout << "min_vote_count: " <<  min_vote_count << endl;
    return min_vote_count;
  }

  // this method returns a list of pointers to all Candidate's whose ballot count
  // equals min_vote_count, aka all losers. the returned list will be used to:
  // (1) collect all ballots of all losers (so the losers can be removed from
  // these ballots); and (2) eliminate the losers so they won't feature in
  // future rounds of min_vote_count().
  // class Candidates
  CandidatePointers min_vote_candidates(int min_vote_count)
  {
    CandidatePointers candidates(&list[0]);
    for (vector<Candidate>::iterator it = list.begin(); it != list.end(); it++)
    {
      // a candidate whose total vote count equals the minimum vote count
      if (it->ballots.list.size() == min_vote_count)
        candidates.list.push_back(&*it);
    }
    return candidates;
  }

  // this method re-circulates the eliminated candidates' ballots by adding
  // those ballots into the list of active candidates
  // class Candidates
  void recount_ballots(BallotPointers& ballots)
  {
    for (int i = 0; i < list.size(); i++)
    {
      Candidate* candidate = &list[i];
      // add all ballots whose first vote is the current candidate to the
      // candidate's list of current ballots
      candidate->add_ballots(ballots.match(i + 1));
    }
  }

  int size()
  {
    int size = 0;
    for (Candidate candidate : list)
      size += candidate.size();
    return size;
  }

  // this method goes thru the current list of candidates to find those whose
  // ranking (position/index) matches the indices, collects and returns those
  // candidates
  // class Candidates
  Candidates losers(Indices& losers_indices)
  {
    Candidates losers;
    for (int i = 0; i < list.size(); i++)
      for (vector<int>::iterator it = losers_indices.list.begin();
           it != losers_indices.list.end();
           it++)
        if (i+1 == *it)
          losers.list.push_back(list[i]);
    return losers;
  }
};

class Poll
{
public:
  vector<string> names;
  Ballots ballots;

  operator string()
  {
    string builder;
    builder.append(to_string(names.size())).append(" candidates:\n");
    for (vector<string>::iterator it = names.begin(); it != names.end(); it++)
      builder.append(" <").append(*it).append(">\n");

    builder.append(to_string(ballots.list.size())).append(" ballots:\n");
    builder.append(ballots);
    return builder;
  }
};

vector<Poll> input()
{
  vector<Poll> list;
  string line;
  getline(cin, line);
  int case_count = stoi(line);

  // skip a blank line
  getline(cin, line);

  for (int i = 0; i < case_count; i++)
  {
    getline(cin, line);
    int candidate_count = stoi(line);

    // collect all names
    Poll poll;
    for (int j = 0; j < candidate_count; j++)
    {
      getline(cin, line);
      poll.names.push_back(line);
    }

    while (getline(cin, line))
    {
      // read until end of current poll
      if (line.length() == 0)
        break;
      else
        // collect all ballots
        poll.ballots.list.push_back(Ballot(line));
    }

    list.push_back(poll);
  }

  return list;
}

void output(vector<Poll>& list)
{
  // reminder: a Poll is a list of names and of ballots
  for (vector<Poll>::iterator it = list.begin(); it != list.end(); it++)
  {
    // cout << "POLL\n" + it->operator string();
    Candidates all_candidates;
    int ballot_count = it->ballots.list.size();

    // for each name in the current poll
    for (int j = 0; j < it->names.size(); j++)
    {
      string name = it->names[j];
      // create a candidate with such a name
      Candidate candidate(name);
      // collect those ballots whose first vote match the current candidate,
      // based on the candidate's current position (index). the master list of
      // ballots remains unchanged.
      candidate.ballots = it->ballots.match(j + 1);

      // cout << "Candidate " << (j+1) << ": " + candidate.operator string();
      // save the candidate
      all_candidates.list.push_back(candidate);
    }

    // keep looking for a winner, one with more than 50% of the vote
    Candidate* winner = all_candidates.find_winner(ballot_count);
    while (winner == NULL)
    {
      // begin the process of elimination: start by looking for the lowest
      // number of votes received by each candidate
      int min_vote_count = all_candidates.min_vote_count();
      // obtain the position (index) of the candidates with the lowest number
      // of votes.
      // collect all actual candidates with the lowest number of votes.
      CandidatePointers losing_candidates = all_candidates.min_vote_candidates(min_vote_count);
      Indices losers_indices = losing_candidates.indices();
      // cout << "Losers' indices:" << losers_indices.operator string() << endl;
      // cout << "Losers:" << endl << losing_candidates.operator string();
      // collect all ballots belonging to the losing candidates
      BallotPointers losers_ballots = losing_candidates.all_ballots();
      // cout << "All losers' ballots:\n" << losers_ballots.operator string();
      // eliminate the losing candidates from the master list of ballots
      it->ballots.remove_votes(losers_indices);
      // cout << "All losers' ballots, refreshed:\n" << losers_ballots.operator string();
      // eliminate these candidates from the voting poll
      losing_candidates.eliminate();
      // recount the ballots in favor of their highest-ranked non-eliminated
      // candidate
      all_candidates.recount_ballots(losers_ballots);
      // keep looking for a winner
      winner = all_candidates.find_winner(ballot_count);
    }
    cout << winner->name << endl;
  }
}

int main()
{
  vector<Poll> list = input();
  output(list);
}
