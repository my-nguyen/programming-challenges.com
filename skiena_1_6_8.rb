class Ballot
  attr_accessor :votes

  # this constructor initializes the ballot with integers extracted from a string
  def initialize(line)
    @votes = []
    line.split.each do |token|
      @votes << token.to_i
    end
  end

  # this method removes all votes that match the list of eliminated candidates
  def remove_votes(losers_indices)
    # must use the following block construct to remove an element from List
    # while iterating over it.
    @votes.delete_if do |vote|
      losers_indices.include?(vote)
    end
  end
end

class BallotList
  attr_accessor :list

  def initialize
    @list = []
  end

  def to_s
    builder = ""
    @list.each do |ballot|
      builder << " " << ballot.votes.to_s
    end
    builder
  end

  def remove_ballots(losers_indices)
    @list.each do |ballot|
      ballot.remove_votes(losers_indices)
    end
  end

  # this method goes through a list of ballots and extract those ballots whose
  # first choice match the current candidate before returning the extracted list
  def extract_ballots(choice)
    extract = BallotList.new
    @list.delete_if do |ballot|
      if ballot.votes[0] == choice
        extract.list << ballot
        true
      end
    end
    extract
  end

  def assign(ballots)
    @list.clear
    ballots.list.each do |ballot|
      @list << ballot
    end
  end
end

class Candidate
  attr_accessor :name, :ballots

  def initialize(nom)
    @name = nom
    @ballots = BallotList.new
  end

  def add_ballots(new_ballots)
    if (@ballots.list.size != 0 && new_ballots.list.size != 0)
      new_ballots.list.each do |ballot|
        @ballots.list << ballot
      end
    end
  end

  def remove_ballots(indices)
    @ballots.remove_ballots(indices)
  end

  def to_s
    builder = ""
    builder << "<" << name << "> "
    builder << @ballots.list.size.to_s << " ballots\n"
    builder << @ballots.to_s
  end
end

class CandidateList
  attr_accessor :list

  def initialize
    @list = []
  end

  def to_s
    builder = ""
    @list.each do |candidate|
      builder << candidate.to_s
    end
    builder
  end

  # this method looks in the list of remaining candidates and checks if they
  # are all of the same size. if so they are collected into a list and returned
  # otherwise null is returned.
  def unisize
    # look for the first candidate with non-zero ballots
    i = 0
    while (i < @list.size && @list[i].ballots.list.size == 0)
      i += 1
    end
    result = []
    size = @list[i].ballots.list.size
    result << @list[i]

    # look in the rest of list of candidates
    (i+1).upto(list.size-1) do |j|
      if (@list[j].ballots.list.size != 0)
        # if any candidate doesn't have the same size, return an empty list
        if (@list[j].ballots.list.size != size)
          return nil
        # if another candidate is of the same size, it is collected
        else
          result << @list[j]
        end
      end
    end

    result
  end

  # this method finds a candidate whose total ballot count is more than 50% of
  # the total ballot count
  # class Candidates
  def find_winners(ballot_count)
    result = unisize
    # not all remaining candidates have the same size
    if (result == nil)
      @list.size.times do |i|
        candidate = @list[i]
        # puts("Total ballots: #{ballot_count}, candidate: #{(i+1)}, ballot count: #{candidate.ballots.list.size}")
        # look for candidate with ballot count more than 50% of the total votes
        if (candidate.ballots.list.size / ballot_count.to_f >= 0.5)
          result = [candidate]
          break
        end
      end
    end
    result
  end

  # this method returns the lowest number of votes among all candidates
  def min_vote_count
    min_vote_count = 1000
    @list.each do |candidate|
      vote_count = candidate.ballots.list.size
      if (vote_count != 0 && vote_count < min_vote_count)
        min_vote_count = vote_count
      end
    end

    # puts("min_vote_count: #{min_vote_count}")
    min_vote_count
  end

  # this method returns a list of index to all Candidate's whose ballot count
  # equals min_vote_count, aka all losers. the returned list will be used to:
  # (1) collect all ballots of all losers (so the losers can be removed from
  # these ballots); and (2) eliminate the losers so they won't feature in
  # future rounds of min_vote_count(). in a previous implementation, a list of
  # Candidate's was returned to perform steps (1) and (2) above. however, after
  # collecting all ballots of all losers, the first element of each ballot was
  # simply removed, which yielded incorrect result. for example, in one round,
  # the losers tie for lowest votes of 4 have their index at 1, 5, 8, 11. These
  # 4 losers have a combined list of 16 ballots, some of which are [5, 11, 8, 9],
  # [5, 1, 9, 6], [8, 5, 13, 4], [8, 5, 3, 2]. removing the first element from
  # each ballots will result: [11, 8, 9], [1, 9, 6], [5, 13, 4], [5, 3, 2]
  # which is incorrect. in this implementation, a list of indices insteall will
  # additionally serve to keep removing the losers from the 16 ballots (as
  # opposed to just removing the first element), so that the ballots will end
  # up with correct result: [9], [9, 6], [13, 4], [3, 2].
  def min_vote_indices(min_vote_count)
    indices = []
    @list.each_index do |i|
      # a candidate whose vote counts equals the minimum vote count
      if (@list[i].ballots.list.size == min_vote_count)
        indices << (i + 1)
      end
    end
    indices
  end

  # this method returns a list of all ballots of each candidate in the list of
  # candidates
  def ballots
    result = BallotList.new
    @list.each do |candidate|
      candidate.ballots.list.each do |ballot|
        result.list << ballot
      end
    end

    result
  end

  # this method adds a new list of ballots into the current list of candidates
  def add_ballots(ballots)
    @list.each_index do |i|
      candidate = @list[i]
      # add all ballots whose first vote is the current candidate to the
      # candidate's list of current ballots
      candidate.add_ballots(ballots.extract_ballots(i + 1))
    end
  end

  def remove_ballots(indices)
    @list.each do |candidate|
      candidate.remove_ballots(indices)
    end
  end

  # this method removes all ballots for each candidate in the list of candidates
  def eliminate
    @list.each do |candidate|
      candidate.ballots.list.clear
    end
  end

  # this method goes thru the current list of candidates to find those whose
  # ranking (position/index) matches the indices, collects and returns those
  # candidates
  def losers(losers_indices)
    losers = CandidateList.new
    @list.each_index do |i|
      losers_indices.each do |j|
        if (i+1 == j)
          losers.list << @list[i]
          # puts("added #{@list[i]}")
        end
      end
    end
    losers
  end
end

class Poll
  attr_accessor :names, :ballots

  def initialize
    @names = []
    @ballots = BallotList.new
  end

  def to_s
    builder = ""
    builder << @names.size.to_s << " candidates:\n"
    @names.each do |name|
      builder << " <" << name << ">\n"
    end

    builder << @ballots.list.size.to_s << " ballots:\n"
    builder << @ballots.to_s
  end
end

def input
  list = []
  case_count = gets.to_i

  # skip a blank line
  gets

  case_count.times do |i|
    candidate_count = gets.to_i

    # collect all names
    poll = Poll.new
    candidate_count.times do |j|
      poll.names << gets.chomp
    end

    while (line = gets)
      line.chomp!
      # collect all ballots
      # end of current poll
      if (line.length == 0)
        break
      else
        poll.ballots.list << Ballot.new(line)
      end
    end

    list << poll
  end

  list
end

def reshuffle(candidates, vote_count)
  # obtain the position (index) of the candidates with the lowest number
  # of votes.
  losers_indices = candidates.min_vote_indices(vote_count)
  # puts("Losers' indices: #{losers_indices}")
  # collect all actual candidates with the lowest number of votes.
  losers = candidates.losers(losers_indices)
  # print("Losers:\n#{losers}")
  # collect all ballots belonging to these candidates
  losers_ballots = losers.ballots()
  # print("Losers' ballots, pre-removal:\n#{losers_ballots}")
  # eliminate these candidates from these ballots
  candidates.remove_ballots(losers_indices)
  # print("Losers' ballots, post-removal:\n#{losers_ballots}")
  # recount the ballots in favor of their highest-ranked non-eliminated
  # candidate
  candidates.add_ballots(losers_ballots)
  losers
end

def output(list)
  # reminder: a Poll is a list of names and of ballots
  list.each do |poll|
    # print("POLL\n#{poll}")
    candidates = CandidateList.new

    ballot_count = poll.ballots.list.size
    # for each name in the current poll
    poll.names.size.times do |j|
      name = poll.names[j]
      # create a candidate with such a name
      candidate = Candidate.new(name)
      # and with ballots extracted also from the poll, based on the current
      # position (index) of the candidate
      candidate.ballots.assign(poll.ballots.extract_ballots(j + 1))

      # print("Candidate #{(j+1)}: #{candidate}")
      # save the candidate
      candidates.list << candidate
    end

    reshuffle(candidates, 0)

    # keep looking for a winner, one with more than 50% of the vote
    winners = candidates.find_winners(ballot_count)
    while (winners == nil)
      # begin the process of elimination: start by looking for the lowest
      # number of votes received by each candidate
      min_vote_count = candidates.min_vote_count()
      losers = reshuffle(candidates, min_vote_count)
      # eliminate these candidates from the voting poll
      losers.eliminate()
      # keep looking for a winner
      winners = candidates.find_winners(ballot_count)
    end

    # print the winners' name, as per problem spec
    winners.each do |winner|
      puts winner.name
    end
    # blank line to separate consecutive cases, as per problem spec
    puts
  end
end

output(input())
