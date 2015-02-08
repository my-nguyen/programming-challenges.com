import java.io.File;
import java.io.FileNotFoundException;
import java.util.Scanner;
import java.util.List;
import java.util.ArrayList;
import java.util.Arrays;


class sol_1_6_8 {
   private static void input(List<Input> input) throws FileNotFoundException {
      // read the commmands and their corresponding parameters until
      // end of input
      Scanner scanner = new Scanner(System.in);


      // read the number of test cases
      int num_cases = Integer.parseInt(scanner.nextLine());


      // read a blank line
      scanner.nextLine();


      for (int i = 0; i < num_cases; i++)
      {
         // read the count of the candidates
         Input current = new Input();
         int num_candidates = Integer.parseInt(scanner.nextLine());


         // read all candidates' names
         current._names._list = new ArrayList<String>();
         for (int j = 0; j < num_candidates; j++)
            current._names._list.add(scanner.nextLine());


         // read all ballots, one line at a time. stop when EOF or
         // empty line is fetched
         while (scanner.hasNext()) {
            String line = scanner.nextLine();
            if (line.length() == 0)
               break;
            else
               current._ballots.add(line);
         }

         input.add(current);
      }

      scanner.close();
   }


   private static void output(List<Input> input) {
      for (int i = 0; i < input.size(); i++)
      {
         CandidateList candidates = new CandidateList();
         Input current = input.get(i);


         // go through the list of candidates' names
         for (int j = 0; j < current._names._list.size(); j++)
         {
            // save the input candidate's name
            Candidate candidate = new Candidate(current._names._list.get(j),
                                       true, new BallotList());


            // go through the list of ballots and record all the ballots
            // whose first choice is the current candidate
            for (int k = 0; k < current._ballots._list.size(); k++)
               if (current._ballots._list.get(k)._list.get(0) == j+1)
                  candidate._ballots.add(current._ballots._list.get(k));

            // save the current candidate
            candidates._list.add(candidate);
         }

         // while unable to find the winner
         while (candidates.findWinner(current._ballots._list.size()) == false) {
            // find the losers
            int min_votes = current._ballots._list.size();
            IndexList indices = new IndexList();
            for (int k = 0; k < candidates._list.size(); k++)
            {
               if (candidates._list.get(k)._active)
               {
                  if (candidates._list.get(k)._ballots._list.size() < min_votes)
                  {
                     indices._list.clear();
                     indices._list.add(k);
                     min_votes = candidates._list.get(k)._ballots._list.size();
                  }
                  else if (candidates._list.get(k)._ballots._list.size() == min_votes)
                     indices._list.add(k);
               }
            }

            // deactive the losers
            for (int k = 0; k < indices._list.size(); k++)
               candidates._list.get(indices._list.get(k))._active = false;


            for (int k = 0; k < indices._list.size(); k++)
            {
               // from the list of ballots of the deactivated losers,
               // promote candidates from second choice to first choice
               int idx = indices._list.get(k);
               BallotList ballots = candidates._list.get(idx)._ballots;
               ballots.shiftLeft(candidates);

               // add all ballots of the newly-promoted first choice
               // candidates to the existing first-choicers
               for (int l = 0; l < candidates._list.size(); l++)
                  candidates._list.get(l).addBallots(ballots);

               // reset the ballot count for the current deactivated loser
               ballots._list.clear();
            }
         }
      }
   }


   public static void main(String[] args) throws FileNotFoundException {
      List<Input> list = new ArrayList<Input>();
      input(list);
      output(list);
   }
}


class NameList {
   List<String> _list = new ArrayList<String>();


   // print this list of names
   void print() {
      for (int i = 0; i < _list.size(); i++)
         System.out.print(_list.get(i) + "\n");
   }
}


class Ballot {
   List<Integer> _list = new ArrayList<Integer>();


   // initialize this ballot with integers extracted from a string
   Ballot(String line) {
      // use a temporary copy to preserve the input line
      String[] tokens = line.split(" ");
      for (int i = 0; i < tokens.length; i++)
         _list.add(Integer.parseInt(tokens[i]));
   }


   // initialize this ballot with another ballot
   Ballot(Ballot that) {
      for (int i = 0; i < that._list.size(); i++)
         _list.add(that._list.get(i));
   }


   // print this ballot
   void print() {
      for (int i = 0; i < _list.size(); i++)
         System.out.print(_list.get(i) + " ");
      System.out.print("\n");
   }


   // shift all integers in this ballot to the left, so that the leftmost
   // integer becomes the rightmost integer, and all other integers move
   // one position to the left
   void shiftLeft() {
      int out = _list.get(0);
      for (int i = 0; i < _list.size()-1; i++)
         _list.set(i, _list.get(i+1));
      _list.set(_list.size()-1, out);
   }
}


class BallotList {
   List<Ballot> _list = new ArrayList<Ballot>();


   BallotList() {
   }


   // initialize this list of ballots with another
   BallotList(BallotList that) {
      if (that._list.size() == 0)
         _list.clear();
      else {
         for (int i = 0; i < that._list.size(); i++)
            _list.add(that._list.get(i));
      }
   }


   // add one ballot into this list of ballots
   void add(Ballot ballot) {
      _list.add(new Ballot(ballot));
   }


   // go through the list of ballots and record all the ballots whose
   // first choice is the current candidate
   void add(BallotList that) {
      for (int i = 0; i < that._list.size(); i++)
         if (that._list.get(i)._list.get(0) == _list.get(0)._list.get(0))
            add(new Ballot(that._list.get(i)));
   }


   // add one ballot into this list of ballots, based on string contents
   void add(String line) {
      _list.add(new Ballot(line));
   }


   // print all ballots in this list
   void print() {
      for (int i = 0; i < _list.size(); i++) {
         System.out.print("BALLOT " + i + ": ");
         _list.get(i).print();
      }
   }


   // shift all ballots in this list to the left
   void shiftLeft(CandidateList candidates) {
      for (int i = 0; i < _list.size(); i++) {
         int index = _list.get(i)._list.get(0) - 1;
         while (candidates._list.get(index)._active == false)
         {
            _list.get(i).shiftLeft();
            index = _list.get(i)._list.get(0) - 1;
         }
      }
   }
}


class Candidate {
   String    _name = new String();
   boolean   _active = false;
   BallotList _ballots = new BallotList();


   // initialize the current candidate with a name, a status, and a list
   // of ballots
   Candidate(String name, boolean active, BallotList ballots)
   {
      _name = name;
      _active = active;
      _ballots = ballots;
   }


   // add a list of ballots into this candidate
   void addBallots(BallotList ballots)
   {
      if (_active)
         _ballots.add(ballots);
   }


   // print contents of the current candidate
   void print()
   {
      System.out.print(_name + ", active " + _active +
                   ", votes " + _ballots._list.size() + "\n");
   }
}


class CandidateList {
   List<Candidate> _list = new ArrayList<Candidate>();


   CandidateList() {
   }


   // print all candidates in this list
   void print()
   {
      for (int i = 0; i < _list.size(); i++)
         _list.get(i).print();
   }


   // from among this list of candidates, find a candidate whose total
   // ballot count is more than 50% of the total ballot count
   boolean findWinner(int num_ballots)
   {
      for (int i = 0; i < _list.size(); i++)
         if (_list.get(i)._ballots._list.size() /
            (float)num_ballots >= .5)
         {
            System.out.print(_list.get(i)._name + "\n");
            return true;
         }

      return false;
   }
}


class Input {
   NameList   _names = new NameList();
   BallotList _ballots = new BallotList();


   // print contents of the current test case, including all candidates
   // names and all ballots
   void print()
   {
      System.out.print(_names._list.size() + " CANDIDATES\n");
      _names.print();
      System.out.print(_ballots._list.size() + " BALLOTS\n");
      _ballots.print();
   }
}


class IndexList {
   List<Integer> _list = new ArrayList<Integer>();


   // print the name and ballot count of all the losing candidates
   void print(CandidateList candidates) {
      System.out.print(_list.size() + " LOSERS:\n");
      for (int i = 0; i < _list.size(); i++)
      {
         Candidate candidate = candidates._list.get(_list.get(i));
         candidate.print();
         candidate._ballots.print();
      }
   }
}
