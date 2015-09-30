import java.util.*;

class Ballot
{
  List<Integer> choices = new ArrayList<>();

  // this constructor initializes the ballot with integers extracted from a string
  Ballot(String line)
  {
    String[] tokens = line.split(" ");
    for (String token : tokens)
      choices.add(Integer.parseInt(token));
  }

  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    builder.append("[");
    for (int number : choices)
      builder.append(Integer.toString(number)).append(" ");
    builder.deleteCharAt(builder.length()-1);
    builder.append("]");
    builder.append("\n");
    return builder.toString();
  }

  // this method removes all votes that match the list of eliminated candidates
  void remove_votes(Indices losers_indices)
  {
    // must use the following block construct to remove an element from List
    // while iterating over it.
    Iterator<Integer> it = choices.iterator();
    while (it.hasNext())
    {
      int choice = it.next();
      for (int index : losers_indices.data)
        if (choice == index)
          it.remove();
    }
  }
}

class BallotList
{
  List<Ballot> data = new ArrayList<>();

  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    for (Ballot ballot : data)
      builder.append(" ").append(ballot);
    return builder.toString();
  }

  void remove_ballots(Indices losers_indices)
  {
    for (Ballot ballot : data)
      ballot.remove_votes(losers_indices);
  }

  // this method goes through a list of ballots and extract those ballots whose
  // first choice match the current candidate before returning the extracted list
  BallotList extract_ballots(int choice)
  {
    BallotList extract = new BallotList();
    Iterator<Ballot> it = data.iterator();
    while (it.hasNext())
    {
      Ballot ballot = it.next();
      // first choice in ballot matches the current candidate
      if (ballot.choices.get(0) == choice)
      {
        // retain this ballot for the current candidate
        extract.data.add(ballot);
        // remove the ballot from the list of ballots
        it.remove();
      }
    }
    return extract;
  }
}

class Candidate
{
  String name;
  BallotList ballots;

  Candidate(String nom)
  {
    name = nom;
    ballots = null;
  }

  void add_ballots(BallotList new_ballots)
  {
    if (ballots.data.size() != 0)
      for (Ballot ballot : new_ballots.data)
        ballots.data.add(ballot);
  }

  void remove_ballots(Indices indices)
  {
    ballots.remove_ballots(indices);
  }

  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    builder.append("<").append(name).append("> ");
    builder.append(Integer.toString(ballots.data.size())).append(" ballots\n");
    builder.append(ballots);
    return builder.toString();
  }
}

class CandidateList
{
  List<Candidate> data = new ArrayList<>();

  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    for (Candidate candidate : data)
      builder.append(candidate);
    return builder.toString();
  }

  // this method finds a candidate whose total ballot count is more than 50% of
  // the total ballot count
  Candidate find_winner(int ballot_count)
  {
    for (int i = 0; i < data.size(); i++)
    {
      Candidate candidate = data.get(i);
      // System.out.println("total ballots: " + ballot_count + ", candidate: " + (i+1) + ", ballot count: " + candidate.ballots.data.size());
      // look for candidate with ballot count more than 50% of the total votes
      if (candidate.ballots.data.size() / (float)ballot_count >= .5)
        return candidate;
    }
    return null;
  }

  // this method returns the lowest number of votes among all candidates
  int min_vote_count()
  {
    int min_vote_count = Integer.MAX_VALUE;
    for (Candidate candidate : data)
    {
      int vote_count = candidate.ballots.data.size();
      if (vote_count != 0)
        if (vote_count < min_vote_count)
          min_vote_count = vote_count;
    }

    // System.out.println("min_vote_count: " + min_vote_count);
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
  Indices min_vote_indices(int min_vote_count)
  {
    Indices indices = new Indices();
    for (int i = 0; i < data.size(); i++)
    {
      // a candidate whose vote counts equals the minimum vote count
      if (data.get(i).ballots.data.size() == min_vote_count)
        indices.data.add(i + 1);
    }
    return indices;
  }

  // this method returns a list of all ballots of each candidate in the list of
  // candidates
  BallotList ballots()
  {
    BallotList ballots = new BallotList();
    for (Candidate candidate : data)
      for (Ballot ballot : candidate.ballots.data)
        ballots.data.add(ballot);

    return ballots;
  }

  // this method adds a new list of ballots into the current list of candidates
  void add_ballots(BallotList ballots)
  {
    for (int i = 0; i < data.size(); i++)
    {
      Candidate candidate = data.get(i);
      // add all ballots whose first vote is the current candidate to the
      // candidate's list of current ballots
      candidate.add_ballots(ballots.extract_ballots(i + 1));
    }
  }

  void remove_ballots(Indices indices)
  {
    for (Candidate candidate : data)
      candidate.remove_ballots(indices);
  }

  // this method removes all ballots for each candidate in the list of candidates
  void eliminate()
  {
    for (Candidate candidate : data)
      candidate.ballots.data.clear();
  }

  // this method goes thru the current list of candidates to find those whose
  // ranking (position/index) matches the indices, collects and returns those
  // candidates
  CandidateList losers(Indices losers_indices)
  {
    CandidateList losers = new CandidateList();
    for (int i = 0; i < data.size(); i++)
      for (int j : losers_indices.data)
        if (i+1 == j)
          losers.data.add(data.get(i));
    return losers;
  }
}

class Poll
{
  List<String> names = new ArrayList<>();
  BallotList ballots = new BallotList();

  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    builder.append(Integer.toString(names.size())).append(" candidates:\n");
    for (String name : names)
      builder.append(" <").append(name).append(">\n");

    builder.append(Integer.toString(ballots.data.size())).append(" ballots:\n");
    builder.append(ballots);
    return builder.toString();
  }
}

// this class represents a list of integer indices. its main purpose is for the
// convenience of printing by way of method toString()
class Indices
{
  List<Integer> data = new ArrayList<>();

  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    for (int index : data)
      builder.append(" ").append(Integer.toString(index));
    return builder.toString();
  }
}

class skiena_1_6_8
{
  private static List<Poll> input()
  {
    List<Poll> list = new ArrayList<>();
    Scanner scanner = new Scanner(System.in);
    int case_count = Integer.parseInt(scanner.nextLine());

    // skip a blank line
    scanner.nextLine();

    for (int i = 0; i < case_count; i++)
    {
      int candidate_count = Integer.parseInt(scanner.nextLine());

      // collect all names
      Poll poll = new Poll();
      poll.names = new ArrayList<String>();
      for (int j = 0; j < candidate_count; j++)
        poll.names.add(scanner.nextLine());

      while (scanner.hasNext())
      {
        // collect all ballots
        String line = scanner.nextLine();
        // end of current poll
        if (line.length() == 0)
          break;
        else
          poll.ballots.data.add(new Ballot(line));
      }

      list.add(poll);
    }

    scanner.close();
    return list;
  }

  private static void output(List<Poll> list)
  {
    // reminder: a Poll is a list of names and of ballots
    for (Poll poll : list)
    {
      // System.out.print("POLL\n" + poll);
      CandidateList candidates = new CandidateList();
      int ballot_count = poll.ballots.data.size();

      // for each name in the current poll
      for (int j = 0; j < poll.names.size(); j++)
      {
        String name = poll.names.get(j);
        // create a candidate with such a name
        Candidate candidate = new Candidate(name);
        // and with ballots extracted also from the poll, based on the current
        // position (index) of the candidate
        candidate.ballots = poll.ballots.extract_ballots(j + 1);

        // System.out.print("Candidate " + (j+1) + ": " + candidate);
        // save the candidate
        candidates.data.add(candidate);
      }

      // keep looking for a winner, one with more than 50% of the vote
      Candidate winner = candidates.find_winner(ballot_count);
      while (winner == null)
      {
        // begin the process of elimination: start by looking for the lowest
        // number of votes received by each candidate
        int min_vote_count = candidates.min_vote_count();
        // obtain the position (index) of the candidates with the lowest number
        // of votes.
        Indices losers_indices = candidates.min_vote_indices(min_vote_count);
        // System.out.println("Losers' indices:" + losers_indices);
        // collect all actual candidates with the lowest number of votes.
        CandidateList losers = candidates.losers(losers_indices);
        // System.out.print("Losers: " + losers);
        // collect all ballots belonging to these candidates
        BallotList losers_ballots = losers.ballots();
        // System.out.print("All losers' ballots:\n" + losers_ballots);
        // eliminate these candidates from these ballots
        candidates.remove_ballots(losers_indices);
        // System.out.print("All losers' ballots, refreshed:\n" + losers_ballots);
        // recount the ballots in favor of their highest-ranked non-eliminated
        // candidate
        candidates.add_ballots(losers_ballots);
        // eliminate these candidates from the voting poll
        losers.eliminate();
        // keep looking for a winner
        winner = candidates.find_winner(ballot_count);
      }
      System.out.println(winner.name);
    }
  }

  public static void main(String[] args)
  {
    List<Poll> list = input();
    output(list);
  }
}
