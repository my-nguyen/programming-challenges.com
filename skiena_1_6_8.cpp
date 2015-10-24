// compile: g++ -std=c++11 skiena_1_6_8.cpp (to enable std::to_string())
#include <iostream>
#include <vector>
#include <string>
#include <boost/tokenizer.hpp>
using namespace std;
using namespace boost;

string to_string(vector<int>& list)
{
  string builder;
  builder.append("[");
  for (int i = 0; i < list.size(); i++)
    builder.append(to_string(list[i]+1)).append(" ");
  builder.pop_back();
  builder.append("]");
  return builder;
}

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
    for (int i = 0; i < votes.size(); i++)
      builder.append(to_string(votes[i])).append(" ");
    builder.pop_back();
    builder.append("]");
    builder.append("\n");
    return builder;
  }

  static vector<int> indices;
  // unary function (that accepts an element of vector<int> as argument and
  // returns bool) as required by std::remove_if(). this function must be static
  // so std::remove_if() can call it. this function returns whether an int item
  // is found in a vector<int> (indices)
  // class Ballot
  static bool find_loser(int item)
  {
    bool found = (find(indices.begin(), indices.end(), item-1) != indices.end());
    return found;
  }

  // this method removes all votes that match the list of eliminated candidates
  // class Ballot
  void remove_votes(vector<int>& losers_indices)
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
vector<int> Ballot::indices;

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
    for (int i = 0; i < list.size(); i++)
      builder.append(" ").append(*list[i]);
    return builder;
  }

  // this method is similar in functionality to Ballots::match()
  // class BallotPointers
  BallotPointers match(int vote)
  {
    BallotPointers result;
    for (int i = 0; i < list.size(); i++)
    {
      if (list[i]->votes[0] == vote)
        // retain the address (within vector<Ballot>) of the matching ballot
        result.list.push_back(list[i]);
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
    for (int i = 0; i < list.size(); i++)
      builder.append(" ").append(list[i]);
    return builder;
  }

  // class Ballots
  void remove_votes(vector<int>& losers_indices)
  {
    for (int i = 0; i < list.size(); i++)
      list[i].remove_votes(losers_indices);
  }

  // this method searches through a master list of ballots for those whose first
  // vote match the current candidate and returns a list of pointers to matching
  // ballots. the master list of ballots remains unmodified.
  // class Ballots
  BallotPointers match(int candidate)
  {
    BallotPointers result;
    for (int i = 0; i < list.size(); i++)
    {
      if (list[i].votes[0] == candidate)
        // retain the address (within vector<Ballot>) of the matching ballot
        result.list.push_back(&list[i]);
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
      for (int i = 0; i < new_ballots.list.size(); i++)
        ballots.list.push_back(new_ballots.list[i]);
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
  vector<int> indices()
  {
    vector<int> result;
    for (int i = 0; i < list.size(); i++)
      result.push_back(list[i] - base);
    return result;
  }

  // class CandidatePointers
  operator string()
  {
    string builder;
    for (int i = 0; i < list.size(); i++)
      builder.append(*list[i]);
    return builder;
  }

  // this method returns a list of all ballots of all candidates in this object
  // class CandidatePointers
  BallotPointers all_ballots()
  {
    BallotPointers result;
    for (int i = 0; i < list.size(); i++)
      for (int j = 0; j < list[i]->ballots.list.size(); j++)
        result.list.push_back(list[i]->ballots.list[j]);

    return result;
  }

  // this method removes all ballots for each candidate in the list of candidates
  // class CandidatePointers
  void eliminate()
  {
    for (int i = 0; i < list.size(); i++)
      list[i]->ballots.list.clear();
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
    for (int i = 0; i < list.size(); i++)
      builder.append(list[i]);
    return builder;
  }

  // this method looks in the list of remaining candidates and checks if they
  // are all of the same size. if so they are collected into a list and returned
  // otherwise null is returned.
  vector<Candidate*> unisize()
  {
    // look for the first candidate with non-zero ballots
    int i = 0;
    while (i < list.size() && list[i].ballots.list.size() == 0)
      i++;
    vector<Candidate*> result;
    int size = list[i].ballots.list.size();
    result.push_back(&list[i]);

    // look in the rest of list of candidates
    for (int j = i+1; j < list.size(); j++)
      if (list[j].ballots.list.size() != 0)
        // if any candidate doesn't have the same size, return an empty list
        if (list[j].ballots.list.size() != size)
        {
          result.clear();
          break;
        }
        // if another candidate is of the same size, it is collected
        else
          result.push_back(&list[j]);

    return result;
  }

  // this method finds a candidate whose total ballot count is more than 50% of
  // the total ballot count
  // class Candidates
  vector<Candidate*> find_winners(int ballot_count)
  {
    vector<Candidate*> result = unisize();
    // not all remaining candidates have the same size
    if (result.size() == 0)
    {
      for (int i = 0; i < list.size(); i++)
      {
        Candidate* candidate = &list[i];
        // cout << "Total ballots: " << ballot_count << ", candidate: " << (i+1) << ", ballot count: " << candidate->ballots.list.size() << endl;
        // look for candidate with ballot count more than 50% of the total votes
        if (candidate->ballots.list.size() / (float)ballot_count >= .5)
        {
          result.push_back(candidate);
          break;
        }
      }
    }
    return result;
  }

  // this method returns the lowest number of votes among all candidates
  // class Candidates
  int min_vote_count()
  {
    int min_vote_count = numeric_limits<int>::max();
    for (int i = 0; i < list.size(); i++)
    {
      int vote_count = list[i].ballots.list.size();
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
    for (int i = 0; i < list.size(); i++)
    {
      // a candidate whose total vote count equals the minimum vote count
      if (list[i].ballots.list.size() == min_vote_count)
        candidates.list.push_back(&list[i]);
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
  Candidates losers(vector<int>& losers_indices)
  {
    Candidates losers;
    for (int i = 0; i < list.size(); i++)
      for (int i = 0; i < losers_indices.size(); i++)
        if (i+1 == losers_indices[i])
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
    for (int i = 0; i < names.size(); i++)
      builder.append(" <").append(names[i]).append(">\n");

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

// this method does the bulk of the processing as follows:
// (1) looks for candidates with the lowest number of votes;
// (2) collects all ballots belonging to those candidates;
// (3) removes these candidates' votes from the master list of all ballots;
// (4) recounts the ballots in favor of their highest-ranked non-eliminated
//     candidate
CandidatePointers reshuffle(Candidates& candidates, Ballots& ballots, int vote_count)
{
  // obtain the position (index) of the candidates with the lowest number
  // of votes.
  // collect all actual candidates with the lowest number of votes.
  CandidatePointers losing_candidates = candidates.min_vote_candidates(vote_count);
  vector<int> losers_indices = losing_candidates.indices();
  // cout << "Losers' indices: " << losers_indices.operator string() << endl;
  // cout << "Losers:" << endl << losing_candidates.operator string();
  // collect all ballots belonging to the losing candidates
  BallotPointers losers_ballots = losing_candidates.all_ballots();
  // cout << "Losers' ballots, pre-removal:\n" << losers_ballots.operator string();
  // eliminate the losing candidates from the master list of ballots
  ballots.remove_votes(losers_indices);
  // cout << "Losers' ballots, post-removal:\n" << losers_ballots.operator string();
  // recount the ballots in favor of their highest-ranked non-eliminated
  // candidate
  candidates.recount_ballots(losers_ballots);
  return losing_candidates;
}

void output(vector<Poll> list)
{
  // reminder: a Poll is a list of names and of ballots
  for (int i = 0; i < list.size(); i++)
  {
    Poll& poll = list[i];
    // cout << "POLL\n" + it->operator string();
    Candidates all_candidates;
    int ballot_count = poll.ballots.list.size();

    // for each name in the current poll
    for (int j = 0; j < poll.names.size(); j++)
    {
      string name = poll.names[j];
      // create a candidate with such a name
      Candidate candidate(name);
      // collect those ballots whose first vote match the current candidate,
      // based on the candidate's current position (index). the master list of
      // ballots remains unchanged.
      candidate.ballots = poll.ballots.match(j + 1);

      // cout << "Candidate " << (j+1) << ": " + candidate.operator string();
      // save the candidate
      all_candidates.list.push_back(candidate);
    }

    // first, eliminate those candidates who have no ballots
    reshuffle(all_candidates, poll.ballots, 0);

    // keep looking for a winner, one with more than 50% of the vote
    vector<Candidate*> winners = all_candidates.find_winners(ballot_count);
    while (winners.size() == 0)
    {
      // begin the process of elimination: start by looking for the lowest
      // number of votes received by each candidate
      int min_vote_count = all_candidates.min_vote_count();
      // remove the losing candidates' votes and recount ballots
      CandidatePointers losing_candidates = reshuffle(all_candidates, poll.ballots, min_vote_count);
      // eliminate these candidates from the voting poll
      losing_candidates.eliminate();
      // keep looking for a winner
      winners = all_candidates.find_winners(ballot_count);
    }

    // print the winners' name, as per problem spec
    for (int i = 0; i < winners.size(); i++)
      cout << winners[i]->name << endl;
    // blank line to separate consecutive cases, as per problem spec
    cout << endl;
  }
}

int main()
{
  output(input());
}
