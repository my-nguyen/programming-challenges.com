import sys

class Ballot:
  # this constructor initializes the ballot with integers extracted from a string
  def __init__(self, line):
    self.votes = []
    for token in line.split():
      self.votes.append(int(token))

  def str(self):
    return str(self.votes)

  # this method removes all votes that match the list of eliminated candidates
  def remove_votes(self, losers_indices):
    # must use the following block construct to remove an element from List
    # while iterating over it.
    self.votes = [vote for vote in self.votes if (not vote in losers_indices)]

class BallotList:
  def __init__(self):
    self.list = []

  def str(self):
    builder = ""
    for ballot in self.list:
      builder += ballot.str()
      builder += "\n"
    return builder

  def remove_ballots(self, losers_indices):
    for ballot in self.list:
      ballot.remove_votes(losers_indices)

  # this method goes through a list of ballots and extract those ballots whose
  # first choice match the current candidate before returning the extracted list
  def extract_ballots(self, choice):
    extract = BallotList()
    extract.list = [ballot for ballot in self.list if (ballot.votes[0] == choice)]
    self.list = [ballot for ballot in self.list if (ballot.votes[0] != choice)]
    return extract

  def assign(self, ballots):
    self.list = []
    self.list = ballots.list[:]

class Candidate:
  def __init__(self, nom):
    self.name = nom
    self.ballots = BallotList()

  def add_ballots(self, new_ballots):
    if (len(self.ballots.list) != 0 and len(new_ballots.list) != 0):
      for ballot in new_ballots.list:
        self.ballots.list.append(ballot)

  def remove_ballots(self, indices):
    self.ballots.remove_ballots(indices)

  def str(self):
    builder = ""
    builder += self.name
    builder += "\n"
    builder += self.ballots.str()
    return builder

class CandidateList:
  def __init__(self):
    self.list = []

  def str(self):
    builder = ""
    for candidate in self.list:
      #if (not candidate is None):
      builder += candidate.str()
    return builder

  # this method looks in the list of remaining candidates and checks if they
  # are all of the same size. if so they are collected into a list and returned
  # otherwise null is returned.
  def unisize(self):
    # look for the first candidate with non-zero ballots
    i = 0
    while (i < len(self.list) and len(self.list[i].ballots.list) == 0):
      i += 1

    result = []
    size = len(self.list[i].ballots.list)
    result.append(self.list[i])
    # look in the rest of list of candidates
    for j in range(i+1, len(self.list)):
      if (len(self.list[j].ballots.list) != 0):
        # if any candidate doesn't have the same size, return an empty list
        if (len(self.list[j].ballots.list) != size):
          return None
        # if another candidate is of the same size, it is collected
        else:
          result.append(self.list[j])
    return result

  # this method finds a candidate whose total ballot count is more than 50% of
  # the total ballot count
  # class Candidates
  def find_winners(self, ballot_count):
    result = self.unisize()
    # not all remaining candidates have the same size
    if (result is None):
      for i in range(len(self.list)):
        candidate = self.list[i]
        # print("Total ballots: %d, candidate: %d, ballot count: %d" %(ballot_count, (i+1), len(candidate.ballots.list)))
        # look for candidate with ballot count more than 50% of the total votes
        if (len(candidate.ballots.list) / float(ballot_count) >= 0.5):
          result = [candidate]
          # result.append(candidate)
          break
    return result

  # this method returns the lowest number of votes among all candidates
  def min_vote_count(self):
    min_vote_count = 1000
    for candidate in self.list:
      vote_count = len(candidate.ballots.list)
      if (vote_count != 0 and vote_count < min_vote_count):
        min_vote_count = vote_count

    # print "min_vote_count:", min_vote_count
    return min_vote_count

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
  def min_vote_indices(self, min_vote_count):
    indices = []
    for i in range(len(self.list)):
      # a candidate whose vote counts equals the minimum vote count
      if (len(self.list[i].ballots.list) == min_vote_count):
        indices.append(i + 1)
    return indices

  # this method returns a list of all ballots of each candidate in the list of
  # candidates
  def ballots(self):
    result = BallotList()
    for candidate in self.list:
      for ballot in candidate.ballots.list:
        result.list.append(ballot)
    return result

  # this method adds a new list of ballots into the current list of candidates
  def add_ballots(self, ballots):
    for i in range(len(self.list)):
      candidate = self.list[i]
      # add all ballots whose first vote is the current candidate to the
      # candidate's list of current ballots
      candidate.add_ballots(ballots.extract_ballots(i + 1))

  def remove_ballots(self, indices):
    for candidate in self.list:
      candidate.remove_ballots(indices)

  # this method removes all ballots for each candidate in the list of candidates
  def eliminate(self):
    for candidate in self.list:
      candidate.ballots.list = []

  # this method goes thru the current list of candidates to find those whose
  # ranking (position/index) matches the indices, collects and returns those
  # candidates
  def losers(self, losers_indices):
    losers = CandidateList()
    for i in range(len(self.list)):
      for index in losers_indices:
        if (i+1 == index):
          losers.list.append(self.list[i])
    return losers

class Poll:
  def __init__(self):
    self.names = []
    self.ballots = BallotList()

  def str(self):
    builder = ""
    builder += str(self.names)
    builder += "\n"
    for ballot in self.ballots.list:
      builder += str(ballot.votes)
      builder += "\n"
    return builder

def input():
  list = []
  case_count = int(sys.stdin.readline())

  # skip a blank line
  sys.stdin.readline()

  for i in range(case_count):
    candidate_count = int(sys.stdin.readline())

    # collect all names
    poll = Poll()
    for j in range(candidate_count):
      poll.names.append(sys.stdin.readline().rstrip('\n'))

    while True:
      line = sys.stdin.readline().rstrip("\n")
      # collect all ballots
      # end of current poll
      if (len(line) == 0):
        break
      else:
        poll.ballots.list.append(Ballot(line))

    list.append(poll)
  return list

def reshuffle(candidates, vote_count):
  # obtain the position (index) of the candidates with the lowest number
  # of votes.
  losers_indices = candidates.min_vote_indices(vote_count)
  # print "Losers' indices:", str(losers_indices)
  # collect all actual candidates with the lowest number of votes.
  losers = candidates.losers(losers_indices)
  # print "Losers:\n", losers.str(),
  # collect all ballots belonging to these candidates
  losers_ballots = losers.ballots()
  # print "Losers' ballots, pre-removal:\n", losers_ballots.str(),
  # eliminate these candidates from these ballots
  candidates.remove_ballots(losers_indices)
  # print "Losers' ballots, post-removal:\n", losers_ballots.str(),
  # recount the ballots in favor of their highest-ranked non-eliminated
  # candidate
  candidates.add_ballots(losers_ballots)
  return losers

def output(list):
  # reminder: a Poll is a list of names and of ballots
  for poll in list:
    # print "POLL\n", poll.str(),
    candidates = CandidateList()

    ballot_count = len(poll.ballots.list)
    # for each name in the current poll
    for j in range(len(poll.names)):
      name = poll.names[j]
      # create a candidate with such a name
      candidate = Candidate(name)
      # and with ballots extracted also from the poll, based on the current
      # position (index) of the candidate
      candidate.ballots.assign(poll.ballots.extract_ballots(j + 1))

      # print "Candidate %d: %s" %((j+1), candidate.str()),
      # save the candidate
      candidates.list.append(candidate)

    reshuffle(candidates, 0)

    # keep looking for a winner, one with more than 50% of the vote
    winners = candidates.find_winners(ballot_count)
    while (winners is None):
      # begin the process of elimination: start by looking for the lowest
      # number of votes received by each candidate
      min_vote_count = candidates.min_vote_count()
      losers = reshuffle(candidates, min_vote_count)
      # eliminate these candidates from the voting poll
      losers.eliminate()
      # keep looking for a winner
      winners = candidates.find_winners(ballot_count)

    # print the winners' name, as per problem spec
    for winner in winners:
      print winner.name
    # blank line to separate consecutive cases, as per problem spec
    print

output(input())
