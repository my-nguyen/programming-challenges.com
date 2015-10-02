// compile: g++ -std=c++11 skiena_1_6_7.cpp (to enable std::to_string())
#include <iostream>
#include <vector>
#include <string>
#include <boost/tokenizer.hpp>
using namespace std;
using namespace boost;

// this class represents a list of integer indices. its main purpose is for the
// convenience of printing by way of method toString()
class Indices
{
public:
  vector<int> data;

  operator string()
  {
    string builder;
    for (vector<int>::iterator it = data.begin(); it != data.end(); it++)
      builder.append(" ").append(to_string(*it+1));
    return builder;
  }
};

class Ballot
{
public:
  vector<int> choices;

  // this constructor initializes the ballot with integers extracted from a string
  Ballot(string& line)
  {
    tokenizer<char_separator<char> > tokens(line);
    for (tokenizer<char_separator<char> >::iterator it = tokens.begin();
      it != tokens.end();
      it++)
    {
      int choice = stoi(*it);
      choices.push_back(choice);
    }
  }

  // class Ballot
  operator string()
  {
    string builder;
    builder.append("[");
    for (vector<int>::iterator it = choices.begin(); it != choices.end(); it++)
      builder.append(to_string(*it)).append(" ");
    builder.pop_back();
    builder.append("]");
    builder.append("\n");
    return builder;
  }

  static Indices indices;
  // unary function (that accepts an element of vector<int> as argument and
  // returns bool) as required by std::remove_if(). this function must be static
  // so std::remove_if() can call it.
  // class Ballot
  static bool find_loser(int item)
  {
    bool found = (find(indices.data.begin(), indices.data.end(), item-1) != indices.data.end());
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
    choices.erase(remove_if(choices.begin(), choices.end(), find_loser), choices.end());
  }
};
Indices Ballot::indices;

class BallotPointers
{
public:
  vector<Ballot*> data;

  operator string()
  {
    string builder;
    for (vector<Ballot*>::iterator it = data.begin(); it != data.end(); it++)
      builder.append(" ").append(**it);
    return builder;
  }

  // class BallotPointers
  BallotPointers match(int choice)
  {
    BallotPointers result;
    for (vector<Ballot*>::iterator it = data.begin(); it != data.end(); it++)
    {
      if ((*it)->choices[0] == choice)
        // retain the address (within vector<Ballot>) of the matching ballot
        result.data.push_back(*it);
    }
    return result;
  }
};

class BallotList
{
public:
  vector<Ballot> data;

  operator string()
  {
    string builder;
    for (vector<Ballot>::iterator it = data.begin(); it != data.end(); it++)
      builder.append(" ").append(*it);
    return builder;
  }

  // class BallotList
  void remove_votes(Indices& losers_indices)
  {
    for (vector<Ballot>::iterator it = data.begin(); it != data.end(); it++)
      it->remove_votes(losers_indices);
  }

  // this method searches through a master list of ballots for those whose first
  // choice match the current candidate and returns the list of matching ballots.
  // the master list of ballots remains unmodified.
  // class BallotList
  BallotPointers match(int choice)
  {
    BallotPointers result;
    for (vector<Ballot>::iterator it = data.begin(); it != data.end(); it++)
    {
      if (it->choices[0] == choice)
        // retain the address (within vector<Ballot>) of the matching ballot
        result.data.push_back(&*it);
    }
    return result;
  }
};

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
    return ballots.data.size();
  }

  // class Candidate
  void add_ballots(BallotPointers new_ballots)
  {
    if (ballots.data.size() != 0 && new_ballots.data.size() != 0)
    {
      for (vector<Ballot*>::iterator it = new_ballots.data.begin(); it != new_ballots.data.end(); it++)
        ballots.data.push_back(*it);
    }
  }

  // class Candidate
  operator string()
  {
    string builder;
    builder.append("<").append(name).append("> ");
    builder.append(to_string(ballots.data.size())).append(" ballots\n");
    builder.append(ballots);
    return builder;
  }
};

class CandidatePointers
{
public:
  vector<Candidate*> data;
  Candidate* base;

  CandidatePointers(Candidate* address)
  {
    base = address;
  }

  // class CandidatePointers
  Indices indices()
  {
    Indices result;
    for (vector<Candidate*>::iterator it = data.begin(); it != data.end(); it++)
      result.data.push_back(*it - base);
    return result;
  }

  // class CandidatePointers
  operator string()
  {
    string builder;
    for (vector<Candidate*>::iterator it = data.begin(); it != data.end(); it++)
      builder.append(**it);
    return builder;
  }

  // this method returns a list of all ballots of all candidates in this object
  // class CandidatePointers
  BallotPointers all_ballots()
  {
    BallotPointers result;
    for (vector<Candidate*>::iterator it = data.begin(); it != data.end(); it++)
      for (vector<Ballot*>::iterator jt = (*it)->ballots.data.begin(); jt != (*it)->ballots.data.end(); jt++)
        result.data.push_back(*jt);

    return result;
  }

  // this method removes all ballots for each candidate in the list of candidates
  // class CandidatePointers
  void eliminate()
  {
    for (vector<Candidate*>::iterator it = data.begin(); it != data.end(); it++)
      (*it)->ballots.data.clear();
  }
};

class CandidateList
{
public:
  vector<Candidate> data;

  operator string()
  {
    string builder;
    for (vector<Candidate>::iterator it = data.begin(); it != data.end(); it++)
      builder.append(*it);
    return builder;
  }

  // this method finds a candidate whose total ballot count is more than 50% of
  // the total ballot count
  // class CandidateList
  Candidate* find_winner(int ballot_count)
  {
    for (int i = 0; i < data.size(); i++)
    {
      Candidate* candidate = &data[i];
      // cout << "total ballots: " << ballot_count << ", candidate: " << (i+1) << ", ballot count: " << candidate->ballots.data.size() << endl;
      // look for candidate with ballot count more than 50% of the total votes
      if (candidate->ballots.data.size() / (float)ballot_count >= .5)
        return candidate;
    }
    return NULL;
  }

  // this method returns the lowest number of votes among all candidates
  // class CandidateList
  int min_vote_count()
  {
    int min_vote_count = numeric_limits<int>::max();
    for (vector<Candidate>::iterator it = data.begin(); it != data.end(); it++)
    {
      int vote_count = it->ballots.data.size();
      if (vote_count != 0)
        if (vote_count < min_vote_count)
          min_vote_count = vote_count;
    }

    // cout << "min_vote_count: " <<  min_vote_count << endl;
    return min_vote_count;
  }

  // this method returns a list of index to all Candidate's whose ballot count
  // equals min_vote_count, aka all losers. the returned list will be used to:
  // (1) collect all ballots of all losers (so the losers can be removed from
  // these ballots); and (2) eliminate the losers so they won't feature in
  // future rounds of min_vote_count(). in a previous implementation, a list of
  // Candidate's was returned to perform steps (1) and (2) above. however, after
  // collecting all ballots of all losers, the first element of each ballot was
  // simply removed, which yielded incorrect result. for example, in one round,
  // the losers tie for lowest votes of 4 have their index at 1, 5, 8, 11. These
  // 4 losers have a combined list of 16 ballots, some of which are [5, 11, 8, 9],
  // [5, 1, 9, 6], [8, 5, 13, 4], [8, 5, 3, 2]. removing the first element from
  // each ballots will result: [11, 8, 9], [1, 9, 6], [5, 13, 4], [5, 3, 2]
  // which is incorrect. in this implementation, a list of indices insteall will
  // additionally serve to keep removing the losers from the 16 ballots (as
  // opposed to just removing the first element), so that the ballots will end
  // up with correct result: [9], [9, 6], [13, 4], [3, 2].
  // class CandidateList
  CandidatePointers min_vote_candidates(int min_vote_count)
  {
    CandidatePointers candidates(&data[0]);
    for (vector<Candidate>::iterator it = data.begin(); it != data.end(); it++)
    {
      // a candidate whose total vote count equals the minimum vote count
      if (it->ballots.data.size() == min_vote_count)
        candidates.data.push_back(&*it);
    }
    return candidates;
  }

  // this method adds a new list of ballots into the current list of candidates
  // class CandidateList
  void recount_ballots(BallotPointers& ballots)
  {
    for (int i = 0; i < data.size(); i++)
    {
      Candidate* candidate = &data[i];
      // add all ballots whose first vote is the current candidate to the
      // candidate's list of current ballots
      candidate->add_ballots(ballots.match(i + 1));
    }
  }

  int size()
  {
    int size = 0;
    for (Candidate candidate : data)
      size += candidate.size();
    return size;
  }

  // this method goes thru the current list of candidates to find those whose
  // ranking (position/index) matches the indices, collects and returns those
  // candidates
  // class CandidateList
  CandidateList losers(Indices& losers_indices)
  {
    CandidateList losers;
    for (int i = 0; i < data.size(); i++)
      for (vector<int>::iterator it = losers_indices.data.begin();
           it != losers_indices.data.end();
           it++)
        if (i+1 == *it)
          losers.data.push_back(data[i]);
    return losers;
  }
};

class Poll
{
public:
  vector<string> names;
  BallotList ballots;

  operator string()
  {
    string builder;
    builder.append(to_string(names.size())).append(" candidates:\n");
    for (vector<string>::iterator it = names.begin(); it != names.end(); it++)
      builder.append(" <").append(*it).append(">\n");

    builder.append(to_string(ballots.data.size())).append(" ballots:\n");
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
        poll.ballots.data.push_back(Ballot(line));
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
    CandidateList candidates;
    int ballot_count = it->ballots.data.size();

    // for each name in the current poll
    for (int j = 0; j < it->names.size(); j++)
    {
      string name = it->names[j];
      // create a candidate with such a name
      Candidate candidate(name);
      // collect those ballots whose first choice match the current candidate,
      // based on the candidate's current position (index). the master list of
      // ballots remains unchanged.
      candidate.ballots = it->ballots.match(j + 1);

      // cout << "Candidate " << (j+1) << ": " + candidate.operator string();
      // save the candidate
      candidates.data.push_back(candidate);
    }

    // keep looking for a winner, one with more than 50% of the vote
    Candidate* winner = candidates.find_winner(ballot_count);
    while (winner == NULL)
    {
      // begin the process of elimination: start by looking for the lowest
      // number of votes received by each candidate
      int min_vote_count = candidates.min_vote_count();
      // obtain the position (index) of the candidates with the lowest number
      // of votes.
      // collect all actual candidates with the lowest number of votes.
      CandidatePointers losing_candidates = candidates.min_vote_candidates(min_vote_count);
      Indices losers_indices = losing_candidates.indices();
      // cout << "Losers' indices:" << losers_indices.operator string() << endl;
      // cout << "Losers:" << endl << losing_candidates.operator string();
      // collect all ballots belonging to the losing candidates
      BallotPointers losers_ballots = losing_candidates.all_ballots();
      // cout << "All losers' ballots:\n" << losers_ballots.operator string();
      // eliminate these candidates from all ballots
      it->ballots.remove_votes(losers_indices);
      // cout << "All losers' ballots, refreshed:\n" << losers_ballots.operator string();
      // eliminate these candidates from the voting poll
      losing_candidates.eliminate();
      // recount the ballots in favor of their highest-ranked non-eliminated
      // candidate
      candidates.recount_ballots(losers_ballots);
      // keep looking for a winner
      winner = candidates.find_winner(ballot_count);
    }
    cout << winner->name << endl;
  }
}

int main()
{
  vector<Poll> list = input();
  output(list);
}
