import java.util.*;

class Ballot
{
  List<Integer> choices = new ArrayList<>();

  // initialize this ballot with integers extracted from a string
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

  void remove_head()
  {
    choices.remove(0);
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
    // builder.deleteCharAt(builder.length()-1);
    return builder.toString();
  }

  void remove_head()
  {
    for (Ballot ballot : data)
      ballot.remove_head();
  }

  BallotList extract_ballots(int choice)
  {
    // go through all ballots and retain those whose first choice is the
    // current candidate
    BallotList extract = new BallotList();
    Iterator<Ballot> it = data.iterator();
    while (it.hasNext())
    {
      Ballot ballot = it.next();
      // System.out.print("Ballot: " + ballot);
      // if first choice in ballot matches the current candidate
      if (ballot.choices.get(0) == choice)
      {
        // retain this ballot for the current candidate
        // may have to re-write this to remove ballot from ballots because
        // ballots can contain up to 1000 ballot's
        // System.out.println(": first choice");
        extract.data.add(ballot);
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
  // since CandidateList is a list of Candidate whose index in the list is also
  // the Candidate's voting priority, the list must remain intact to preserve
  // the voting priorities. therefore we need a boolean flag to mark those still
  // active in the voting race and those who have been eliminated (inactive)
  boolean active;

  // initialize the Poll candidate with a name, a status, and a list
  // of ballots
  Candidate(String nom)
  {
    name = nom;
    ballots = null;
    active = true;
  }

  void add_ballots(BallotList new_ballots)
  {
    for (Ballot ballot : new_ballots.data)
      ballots.data.add(ballot);
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

  // from among this list of candidates, find a candidate whose total
  // ballot count is more than 50% of the total ballot count
  Candidate find_winner(int ballot_count)
  {
    // for (Candidate candidate : data)
    for (int i = 0; i < data.size(); i++)
    {
      Candidate candidate = data.get(i);
      System.out.println("total ballots: " + ballot_count + ", candidate: " + (i+1) + ", ballot count: " + candidate.ballots.data.size());
      // look for candidate with ballot count more than 50% of the total votes
      if (candidate.ballots.data.size() / (float)ballot_count >= .5)
        return candidate;
    }
    return null;
  }

  int min_vote_count()
  {
    int min_vote_count = Integer.MAX_VALUE;
    for (Candidate candidate : data)
      if (candidate.active)
      {
        int vote_count = candidate.ballots.data.size();
        if (vote_count < min_vote_count)
          min_vote_count = vote_count;
      }

    System.out.println("min_vote_count: " + min_vote_count);
    return min_vote_count;
  }

  CandidateList min_vote_candidates(int min_vote_count)
  {
    CandidateList losers = new CandidateList();
    /*
    Iterator<Candidate> it = data.iterator();
    while (it.hasNext())
    {
      Candidate candidate = it.next();
      if (candidate.ballots.data.size() == min_vote_count)
      {
        losers.data.add(candidate);
        it.remove();
      }
    }
    */
    for (Candidate candidate : data)
      if (candidate.ballots.data.size() == min_vote_count)
      {
        losers.data.add(candidate);
      }
    return losers;
  }

  BallotList all_ballots()
  {
    BallotList ballots = new BallotList();
    for (Candidate candidate : data)
      for (Ballot ballot : candidate.ballots.data)
        ballots.data.add(ballot);

    return ballots;
  }

  void add_ballots(BallotList ballots)
  {
    for (int i = 0; i < data.size(); i++)
    {
      Candidate candidate = data.get(i);
      candidate.add_ballots(ballots.extract_ballots(i + 1));
    }
  }

  void deactivate()
  {
    for (Candidate candidate : data)
      candidate.active = false;
  }
}

class Poll
{
  List<String> names = new ArrayList<>();
  BallotList ballots = new BallotList();

  // print contents of the Poll test case, including all candidates
  // names and all ballots
  public String toString()
  {
    StringBuilder builder = new StringBuilder();
    builder.append(Integer.toString(names.size())).append(" candidates:\n");
    for (String name : names)
      builder.append(" <").append(name).append(">\n");
    // builder.append("\n");

    builder.append(Integer.toString(ballots.data.size())).append(" ballots:\n");
    builder.append(ballots);
    return builder.toString();
  }
}

class skiena_1_6_8
{
  private static List<Poll> input()
  {
    List<Poll> list = new ArrayList<>();
    Scanner scanner = new Scanner(System.in);
    // read the number of test cases
    int case_count = Integer.parseInt(scanner.nextLine());

    // skip a blank line
    scanner.nextLine();

    for (int i = 0; i < case_count; i++)
    {
      // read the number of the candidates
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
      System.out.print("POLL\n" + poll);
      CandidateList candidates = new CandidateList();
      int ballot_count = poll.ballots.data.size();

      // go through the list of names
      for (int j = 0; j < poll.names.size(); j++)
      {
        String name = poll.names.get(j);
        Candidate candidate = new Candidate(name);
        candidate.ballots = poll.ballots.extract_ballots(j + 1);

        // save the current candidate
        System.out.print("Candidate " + (j+1) + ": " + candidate);
        candidates.data.add(candidate);
      }

      // while unable to find the winner
      Candidate winner = candidates.find_winner(ballot_count);
      System.out.println("Winner: " + winner);
      while (winner == null)
      {
        int min_vote_count = candidates.min_vote_count();
        CandidateList losers = candidates.min_vote_candidates(min_vote_count);
        System.out.print("Losers: " + losers);
        BallotList losers_ballots = losers.all_ballots();
        System.out.print("All losers' ballots:\n" + losers_ballots);
        losers_ballots.remove_head();
        System.out.print("All losers' ballots, refreshed:\n" + losers_ballots);
        candidates.add_ballots(losers_ballots);
        losers.deactivate();
        winner = candidates.find_winner(ballot_count);
      }
      System.out.println("Winner: " + winner);
    }
  }

  public static void main(String[] args)
  {
    List<Poll> list = input();
    output(list);
  }
}
