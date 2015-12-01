package edu.cornell.nlp.namedentity;

import edu.berkeley.nlp.util.*;
import edu.cornell.nlp.namedentity.FileScorer;

import java.util.*;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.io.*;

import com.aliasi.chunk.Chunking;
import com.aliasi.corpus.*;

/**
 * @author Brandon Plaster (Template: Dan Klein)
 */
public class NamedEntityClassifier {

  static final String START_WORD = "<S>";
  static final String STOP_WORD = "</S>";
  static final String START_TAG = "<S>";
  static final String STOP_TAG = "</S>";
  static final String UNKNOWN_WORD = "_UNK_";
  static final String END_WORD = "+end+";
  static final String BEGIN_WORD = "+begin+";
  static final String NAN_TAG = "NAN";
  
//  static final String NEPATTERN = "<.+?>(.+?)</.+?>|('s)|(\\d+[:/.,-]\\S*)|([\"$%])|(\\w+[-]?\\w+\\.?(?!\\s+$))|(\\S\\w*)";
  static final String NEPATTERN = "<.+?>(.+?)</.+?>|([.]+)|([{}()\"%,:?!$-])|(\\d+([:/.,-]\\d+)*)|(\\w+(?='[sS]))|(\\w+'([a-rA-R]|[t-zT-Z])\\w*)|('s)|(\\w+([']|[.](?!\\s*$))?)";
//  static final String L1_PATTERN = "<.+?>(.+?)</.+?>|(\\S+)";
  static final String L2_PATTERN = "";

  /** 
   * MUC Documents
   */
  static class MUCDocument {
	  String fullDoc;
	  String hl;
	  String[] dd;
	  String dl;
	  String txt;
	  List<TaggedSentence> taggedSentences;
	  
	 public MUCDocument(String doc) {
		  fullDoc = doc;
		  taggedSentences = new ArrayList<TaggedSentence>();
		  
		  // Parse Document
		  Pattern hlPattern = Pattern.compile("<HL>(.+?)</HL>", Pattern.DOTALL);
		  Matcher hlMatcher = hlPattern.matcher(fullDoc);
		  if(hlMatcher.find()){
			  hl = hlMatcher.group(1);
		  }
		  
		  Pattern dlPattern = Pattern.compile("<DATELINE>(.+?)</DATELINE>", Pattern.DOTALL);
		  Matcher dlMatcher = dlPattern.matcher(fullDoc);
		  if(dlMatcher.find()){
			  dl = dlMatcher.group(1);
		  }
		  
		  Pattern txtPattern = Pattern.compile("<TXT>(.+?)</TXT>", Pattern.DOTALL);
		  Matcher txtMatcher = txtPattern.matcher(fullDoc);
		  Pattern senPattern = Pattern.compile("<s>(.+?)</s>", Pattern.DOTALL);
		  Pattern nePattern = Pattern.compile(NEPATTERN, Pattern.DOTALL);
		  Pattern neTagPattern = Pattern.compile("\"(\\w+)\"");
		  
		  if(txtMatcher.find()){
			  txt = txtMatcher.group(1);
			  Matcher senMatcher = senPattern.matcher(txt);
			  List <String> wordArray = new ArrayList<String>();
			  List <String> tagArray = new ArrayList<String>();
			  
			  // Iterate over sentences
			  while(senMatcher.find()){
				  Matcher l1_Matcher = nePattern.matcher(senMatcher.group(1));

				  // Iterate over words
				  while(l1_Matcher.find()){
					  if(l1_Matcher.group(1) == null){
						  wordArray.add(l1_Matcher.group(0));
						  tagArray.add(NAN_TAG);
					  } else {
						  Matcher neTagMatcher = neTagPattern.matcher(l1_Matcher.group(0));
						  if(neTagMatcher.find()){
							  String[] words = l1_Matcher.group(1).trim().split(" ");
							  for(String word : words){
								  wordArray.add(word);
								  tagArray.add(neTagMatcher.group(1));
							  }
						  }
					  }
				  }
				  
				  // Add sentence to tagged sentences
				  if(!wordArray.isEmpty() && !tagArray.isEmpty()){
					  List <String> words = new BoundedList<String>(new ArrayList<String>(wordArray), START_WORD, STOP_WORD);
					  List <String> tags = new BoundedList<String>(new ArrayList<String>(tagArray), START_TAG, STOP_TAG);
					  taggedSentences.add(new TaggedSentence(words, tags));
					  wordArray.clear();
					  tagArray.clear();
				  }
			  }
		  }		
	 }
	 
	 public List<TaggedSentence> getTaggedSentences(){
		 return taggedSentences;
	 }
	  
  }

  /**
   * Tagged sentences are a bundling of a list of words and a list of their
   * tags.
   */
  static class TaggedSentence {
    List<String> words;
    List<String> tags;

    public int size() {
      return words.size();
    }

    public List<String> getWords() {
      return words;
    }

    public List<String> getTags() {
      return tags;
    }

    public String toString() {
      StringBuilder sb = new StringBuilder();
      for (int position = 0; position < words.size(); position++) {
        String word = words.get(position);
        String tag = tags.get(position);
        sb.append(word);
        sb.append("_");
        sb.append(tag);
      }
      return sb.toString();
    }

    public boolean equals(Object o) {
      if (this == o) return true;
      if (!(o instanceof TaggedSentence)) return false;

      final TaggedSentence taggedSentence = (TaggedSentence) o;

      if (tags != null ? !tags.equals(taggedSentence.tags) : taggedSentence.tags != null) return false;
      if (words != null ? !words.equals(taggedSentence.words) : taggedSentence.words != null) return false;

      return true;
    }

    public int hashCode() {
      int result;
      result = (words != null ? words.hashCode() : 0);
      result = 29 * result + (tags != null ? tags.hashCode() : 0);
      return result;
    }

    public TaggedSentence(List<String> words, List<String> tags) {
      this.words = words;
      this.tags = tags;
    }
  }

  /**
   * States are pairs of tags along with a position index, representing the N
   * tags preceding that position.  So, the START state, which can be gotten by
   * State.getStartState() is [START, START, 0].  To build an arbitrary state,
   * for example [DT, NN, 2], use the static factory method
   * State.buildState("DT", "NN", 2).  There isnt' a single final state, since
   * sentences lengths vary, so State.getEndState(i) takes a parameter for the
   * length of the sentence.
   */
  static class State {

    private static transient Interner<State> stateInterner = new Interner<State>(new Interner.CanonicalFactory<State>() {
      public State build(State state) {
        return new State(state);
      }
    });

    private static transient State tempState = new State();

    public static State getStartState() {
    	List<String> previousTags = new ArrayList<String>(Arrays.asList(START_TAG, START_TAG));
    	return buildState(previousTags, 0);
    }

    public static State getStopState(int position) {
    	List<String> previousTags = new ArrayList<String>(Arrays.asList(STOP_TAG, STOP_TAG));
    	return buildState(previousTags, position);
    }

    public static State buildState(List<String> previousTags, int position) {
      tempState.setState(previousTags, position);
      return stateInterner.intern(tempState);
    }

    public static List<String> toTagList(List<State> states) {
      List<String> tags = new ArrayList<String>();
      if (states.size() > 0) {
    	  tags.add(states.get(0).getPreviousTags().get(states.get(0).getPreviousTags().size() - 1));
    	  for (State state : states) {
    		  tags.add(state.getNthPreviousTag(0));
    	  }
      }
      return tags;
    }

    public int getPosition() {
      return position;
    }
    
    public String getNthPreviousTag(int n) {
        return previousTags.get(n);
      }

    public State getNextState(String tag) {
    	List<String> newPreviousTags = new ArrayList<String>(getPreviousTags());
    	newPreviousTags.add(0, tag);
    	newPreviousTags.remove(newPreviousTags.size() - 1);
    	return State.buildState(newPreviousTags, getPosition() + 1);
    }

    public boolean equals(Object o) {
      if (this == o) return true;
      if (!(o instanceof State)) return false;

      final State state = (State) o;

      if (position != state.position) return false;
      for(int n = 0; n < previousTags.size(); n++){
    	  String tag = previousTags.get(n);
    	  if(tag != null ? !tag.equals(state.getNthPreviousTag(n)) : state.getNthPreviousTag(n) != null) return false;
      }

      return true;
    }

    public int hashCode() {
      int result;
      result = position;
      for(String tag: previousTags){
    	  result = 29 * result + (tag != null ? tag.hashCode() : 0);
      }
      return result;
    }

    public String toString() {
    	String str = "[";
    	for(String tag: previousTags){
    		str += tag + ", ";
    	}
    	str += getPosition() + "]";
      return str;      
    }
    
    public List<String> getPreviousTags(){
    	return previousTags;
    }

    int position;
    List<String> previousTags;

    private void setState(List<String> previousTags, int position) {
      this.previousTags = previousTags;
      this.position = position;
    }

    private State() {
    }

    private State(State state) {
      setState(state.getPreviousTags(), state.getPosition());
    }
  }

  /**
   * A Trellis is a graph with a start state an an end state, along with
   * successor and predecessor functions.
   */
  static class Trellis <S> {
    S startState;
    S endState;
    CounterMap<S, S> forwardTransitions;
    CounterMap<S, S> backwardTransitions;

    /**
     * Get the unique start state for this trellis.
     */
    public S getStartState() {
      return startState;
    }

    public void setStartState(S startState) {
      this.startState = startState;
    }
    
    /**
     * Get the unique end state for this trellis.
     */
    public S getEndState() {
      return endState;
    }

    public void setStopState(S endState) {
      this.endState = endState;
    }

    /**
     * For a given state, returns a counter over what states can be next in the
     * markov process, along with the cost of that transition.  Caution: a state
     * not in the counter is illegal, and should be considered to have cost
     * Double.NEGATIVE_INFINITY, but Counters score items they don't contain as
     * 0.
     */
    public Counter<S> getForwardTransitions(S state) {
      return forwardTransitions.getCounter(state);
    }

    /**
     * For a given state, returns a counter over what states can precede it in
     * the markov process, along with the cost of that transition.
     */
    public Counter<S> getBackwardTransitions(S state) {
      return backwardTransitions.getCounter(state);
    }

    public void setTransitionCount(S start, S end, double count) {
      forwardTransitions.setCount(start, end, count);
      backwardTransitions.setCount(end, start, count);
    }

    public Trellis() {
      forwardTransitions = new CounterMap<S, S>();
      backwardTransitions = new CounterMap<S, S>();
    }
  }

  /**
   * A TrellisDecoder takes a Trellis and returns a path through that trellis in
   * which the first item is trellis.getStartState(), the last is
   * trellis.getEndState(), and each pair of states is connected in the
   * trellis.
   */
  static interface TrellisDecoder <S> {
    List<S> getBestPath(Trellis<S> trellis);
  }

  static class GreedyDecoder <S> implements TrellisDecoder<S> {
    public List<S> getBestPath(Trellis<S> trellis) {
      List<S> states = new ArrayList<S>();
      S currentState = trellis.getStartState();
      states.add(currentState);
      while (!currentState.equals(trellis.getEndState())) {
        Counter<S> transitions = trellis.getForwardTransitions(currentState);
        S nextState = transitions.argMax();
        states.add(nextState);
        currentState = nextState;
      }
      return states;
    }
  }
  
  static class ViterbiDecoder <S> implements TrellisDecoder<S> {
	  
	  public List<S> getBestPath(Trellis<S> trellis) {
		  		  
		  // Create Array of all possible states
		  ArrayList<ArrayList<S>> allStates = new ArrayList<ArrayList<S>>();
		  ArrayList<S> nextStates = new ArrayList<S>();
		  nextStates.add(trellis.getStartState());
		  allStates.add(0, nextStates);
		  while(!nextStates.contains(trellis.getEndState())){
			  ArrayList<S> newStates = new ArrayList<S>();
			  for(S nextState: nextStates){
				  for(S newState: trellis.getForwardTransitions(nextState).keySet()){
					  if(!newStates.contains(newState)) newStates.add(newState);
				  }
			  }
			  allStates.add(newStates);
			  nextStates = newStates;
		  }
		  int T = allStates.size();
		  
		  // Initialize Values
		  ArrayList<int[]> bp = new ArrayList<int[]>();
		  ArrayList<double[]> pi = new ArrayList<double[]>();
		  for(int i = 0; i < T; i++){
			  int size = allStates.get(i).size();
			  int b[] = new int[size];
			  double p[] = new double[size];
			  for(int j = 0; j < size; j++){
				  b[j] = 0;
				  p[j] = Double.NEGATIVE_INFINITY;
			  }
			  bp.add(i, b);
			  pi.add(i, p);
		  }
		  pi.get(0)[0] = 0.0;
		  
		  // Viterbi
		  for(int i = 1; i < T; i++){
			  int xsize = allStates.get(i).size();
			  int ysize = allStates.get(i-1).size();
			  for(int j=0; j<xsize; j++){
				  int b = 0;
				  S s1 = allStates.get(i).get(j);

				  double maxScore = Double.NEGATIVE_INFINITY;
				  for(int k=0; k<ysize; k++){
					  double p = pi.get(i-1)[k];
					  double score = Double.NEGATIVE_INFINITY;
					  S s2 = allStates.get(i-1).get(k);
					  if(trellis.getForwardTransitions(s2).containsKey(s1)){
						  score = trellis.getForwardTransitions(s2).getCount(s1) + p; 
						  if(score > maxScore){
							  maxScore = score;
							  b = k;
						  }
					  }
				  }
				  pi.get(i)[j] = maxScore;
				  bp.get(i)[j] = b;
			  }
		  }
		  
		  // Find max back pointer
		  int currentBP = 0;
		  double maxPi = Double.NEGATIVE_INFINITY;
		  for(int i = 0; i < pi.get(T-1).length; i++){
			  double p = pi.get(T-1)[i];
			  if(p > maxPi){
				  maxPi = p;
				  currentBP = i;
			  }
		  }
		  
		  // Determine Max Path
		  List<S> path = new ArrayList<S>();
		  for(int i = 1; i<=T; i++){
			  path.add(0, allStates.get(T-i).get(currentBP));
			  currentBP = bp.get(T-i)[currentBP];
		  }

		  return path;
	  }
  }

  static class NETagger {

    LocalNGramScorer localNGramScorer;
    TrellisDecoder<State> trellisDecoder;

    // chop up the training instances into local contexts and pass them on to the local scorer.
    public void train(List<TaggedSentence> taggedSentences) {
    	localNGramScorer.train(extractLabeledLocalNGramContexts(taggedSentences));
    }

    // chop up the validation instances into local contexts and pass them on to the local scorer.
    public void validate(List<TaggedSentence> taggedSentences) {
    	localNGramScorer.validate(extractLabeledLocalNGramContexts(taggedSentences));
    }

    private List<LabeledLocalNGramContext> extractLabeledLocalNGramContexts(List<TaggedSentence> taggedSentences) {
      List<LabeledLocalNGramContext> localNGramContexts = new ArrayList<LabeledLocalNGramContext>();
      for (TaggedSentence taggedSentence : taggedSentences) {
        localNGramContexts.addAll(extractLabeledLocalNGramContexts(taggedSentence));
      }
      return localNGramContexts;
    }

    private List<LabeledLocalNGramContext> extractLabeledLocalNGramContexts(TaggedSentence taggedSentence) {
      List<LabeledLocalNGramContext> labeledLocalNGramContexts = new ArrayList<LabeledLocalNGramContext>();
      List<String> words = taggedSentence.getWords();
      List<String> tags = taggedSentence.getTags();
      for (int position = 0; position <= taggedSentence.size() + 1; position++) {
    	  List<String> previousTags = new ArrayList<String>();
    	  for(int n = 1; n < localNGramScorer.getNGramMax(); n++){
    		  previousTags.add(tags.get(position - n));
    	  }
    	  labeledLocalNGramContexts.add(new LabeledLocalNGramContext(words, position, new BoundedList<String>(previousTags, null, tags.get(-1)), tags.get(position)));
      }
      return labeledLocalNGramContexts;
    }

    /**
     * Builds a Trellis over a sentence, by starting at the state State, and
     * advancing through all legal extensions of each state already in the
     * trellis.  You should not have to modify this code (or even read it,
     * really).
     */
    private Trellis<State> buildTrellis(List<String> sentence) {
    	Trellis<State> trellis = new Trellis<State>();
    	trellis.setStartState(State.getStartState());
    	State stopState = State.getStopState(sentence.size() + 2);
    	trellis.setStopState(stopState);
    	Set<State> states = Collections.singleton(State.getStartState());
    	for (int position = 0; position <= sentence.size() + 1; position++) {
    		Set<State> nextStates = new HashSet<State>();
    		for (State state : states) {
    			if (state.equals(stopState))
    				continue;
    			LocalNGramContext localNGramContext = new LocalNGramContext(sentence, position, new BoundedList<String>(state.getPreviousTags(), null, START_TAG));
    			Counter<String> tagScores = localNGramScorer.getLogScoreCounter(localNGramContext);
    			for (String tag : tagScores.keySet()) {
    				double score = tagScores.getCount(tag);
    				State nextState = state.getNextState(tag);
    				trellis.setTransitionCount(state, nextState, score);
    				nextStates.add(nextState);
    			}
    		}
    		//        System.out.println("States: "+nextStates);
    		states = nextStates;
    	}
    	return trellis;
    }

    // to tag a sentence: build its trellis and find a path through that trellis
    public List<String> tag(List<String> sentence) {
      Trellis<State> trellis = buildTrellis(sentence);
      List<State> states = trellisDecoder.getBestPath(trellis);
      List<String> tags = State.toTagList(states);
      tags = stripBoundaryTags(tags);
      return tags;
    }

    /**
     * Scores a tagging for a sentence.  Note that a tag sequence not accepted
     * by the markov process should receive a log score of
     * Double.NEGATIVE_INFINITY.
     */
    public double scoreTagging(TaggedSentence taggedSentence) {
      double logScore = 0.0;
      List<LabeledLocalNGramContext> labeledLocalNGramContexts = extractLabeledLocalNGramContexts(taggedSentence);
      for (LabeledLocalNGramContext labeledLocalNGramContext : labeledLocalNGramContexts) {
        Counter<String> logScoreCounter = localNGramScorer.getLogScoreCounter(labeledLocalNGramContext);
        String currentTag = labeledLocalNGramContext.getCurrentTag();
        if (logScoreCounter.containsKey(currentTag)) {
          logScore += logScoreCounter.getCount(currentTag);
        } else {
          logScore += Double.NEGATIVE_INFINITY;
        }
      }
      return logScore;
    }

    private List<String> stripBoundaryTags(List<String> tags) {
    	// TODO validate this for nGram
      return tags.subList(2, tags.size() - 2);
    }

    public NETagger(LocalNGramScorer localNGramScorer, TrellisDecoder<State> trellisDecoder) {
      this.localNGramScorer = localNGramScorer;
      this.trellisDecoder = trellisDecoder;
    }
  }

  /**
   * A LocalNGramContext is a position in a sentence, along with the previous N Tags
   * -- basically a FeatureVector.
   */
  static class LocalNGramContext {
    List<String> words;
    int position;
    List<String> previousTags;

    public List<String> getWords() {
      return words;
    }

    public String getCurrentWord() {
      return words.get(position);
    }

    public int getPosition() {
      return position;
    }
    
    public String getNthPreviousWord(int n) {
    	return words.get(position - n - 1); 
    }
    
    public String getNthPreviousTag(int n) {
      return previousTags.get(n);
    }
    
    public List<String> getNPreviousTags(int n) {
    	if(n <= 0) return new ArrayList<String>();
    	return new ArrayList<String>(previousTags.subList(0, n));
    }
    
    public List<String> getNPreviousWords(int n) {
    	if(n <= 0) return new ArrayList<String>();
    	List<String> previous = new ArrayList<String>(words.subList(position - n, position));
        Collections.reverse(previous);
        
    	return previous;
    }
    
    public List<String> getPreviousTags(){
    	return previousTags;
    }

    public String toString() {
    	String str = "[";
    	for(String tag: previousTags){
    		str += tag + ", ";
    	}
    	str += getCurrentWord() + "]";
      return str;
    }

    public LocalNGramContext(List<String> words, int position, List<String> previousTags) {
      this.words = words;
      this.position = position;
      this.previousTags = previousTags;
    }
  }

  /**
   * A LabeledLocalNGramContext is a context plus the correct tag for that
   * position -- basically a LabeledFeatureVector
   */
  static class LabeledLocalNGramContext extends LocalNGramContext {
    String currentTag;

    public String getCurrentTag() {
      return currentTag;
    }

    public String toString() {
    	String str = "[";
    	for(String tag: previousTags){
    		str += tag + ", ";
    	}
    	str += getCurrentWord() + "_" + getCurrentTag() + "]";
      return str;
    }

    public LabeledLocalNGramContext(List<String> words, int position, List<String> previousTags, String currentTag) {
      super(words, position, previousTags);
      this.currentTag = currentTag;
    }
  }

  /**
   * LocalNGramScorers assign scores to tags occurring in specific
   * LocalNGramContexts.
   */
  static interface LocalNGramScorer {
    /**
     * The Counter returned should contain log probabilities, meaning if all
     * values are exponentiated and summed, they should sum to one (if it's a 
	 * single conditional probability). For efficiency, the Counter can 
	 * contain only the tags which occur in the given context 
	 * with non-zero model probability.
     */	  
    Counter<String> getLogScoreCounter(LocalNGramContext localNGramContext);

    void train(List<LabeledLocalNGramContext> localNGramContexts);

    void validate(List<LabeledLocalNGramContext> localNGramContexts);
    
	void setNGramMax(int nGramMax);
	int getNGramMax();

  }

  
  /*
   * 
   */
  static class HMMTagScorer implements LocalNGramScorer {
	  
	  int nGramMax;
	  boolean restrictNGrams; // if true, assign log score of Double.NEGATIVE_INFINITY to illegal tag NGrams.
	  
	  public void setNGramMax(int nGramMax){
		  this.nGramMax = nGramMax;
	  }
	  
	  public int getNGramMax(){
		  return nGramMax;
	  }
	    
	  public Counter<String> getLogScoreCounter(LocalNGramContext localNGramContext) {
		  int position = localNGramContext.getPosition();
		  boolean isWordFirstWord = (position == 0)? true: false;
		  boolean isPreviousWordFirstWord = (position == 1)? true: false;
		  
		  String word = localNGramContext.getCurrentWord();
		  String previousWord =	localNGramContext.getNthPreviousWord(0);
		  String previousTag = localNGramContext.getNthPreviousTag(0);
		  
		  List<String> previousWords = localNGramContext.getNPreviousWords(nGramMax - 1);
		  List<String> previousTags = localNGramContext.getNPreviousTags(nGramMax - 1);

		  // Check if known word
		  if (!wordCount.containsKey(word)) word = UNKNOWN_WORD;
		  if (!wordCount.containsKey(previousWord)) previousWord = UNKNOWN_WORD;
		  for(int i = 0; i<previousWords.size(); i++){
			  if (!wordCount.containsKey(previousWords.get(i))) previousWords.set(i, UNKNOWN_WORD);
		  }
		  
		  Counter<String> tagCounter = wordsToTags.getCounter(word);

		  WordVec wv1 = new WordVec(word, isWordFirstWord);
		  WordVec wv2 = new WordVec(previousWord, isPreviousWordFirstWord);

		  Counter<String> logScoreCounter = new Counter<String>();
		  	
		  for (String tag : tagCounter.keySet()) {
			  double logScore = Double.NEGATIVE_INFINITY;
			  
			  if(tag.equals(previousTag)){
				  // Probability of word (Non-first-word bigrams)
				  logScore = nonFirstWordBigram(wv1, wv2, tag);
				  
			  } else {
				  
				  // Probability of transition (Name-Class bigrams)
				  if(previousTag == START_TAG) previousWord = END_WORD;
				  logScore = nameClassBigram(tag, previousTags, previousWords);
				  
				  // Probability of word (First-word bigrams)
				  logScore += firstWordBigram(wv1, tag, previousTag);
				  
				  // Probability that last word was end word (Non-first-word bigrams)
				  WordVec wvE = new WordVec(END_WORD, false);
				  logScore += nonFirstWordBigram(wvE, wv2, previousTag);				  
			  }
			  
			  logScoreCounter.setCount(tag, logScore);
		  }
		  
		  return logScoreCounter;
	  }
	  
	  private double firstWordBigram(WordVec wv1, String tag, String previousTag){
		  // Base Model
		  double remainder = 1.0;
		  String bigram = makeNGramString(Arrays.asList(tag, previousTag));
		  double c_t1t2 = nGramsCount.getCounter(bigram).totalCount();
		  double p_wv1_given_t1t2 = 0.0;
		  WordVec wvB = new WordVec(BEGIN_WORD, false);
		  double c_t1wvB = t1wv2_to_wv1.getCounter(tag + " " + wvB.toString()).totalCount();
		  double lambda1 = (1 - c_t1t2/c_t1wvB)*(1/(1+(ncCount/c_t1wvB)));
		  remainder -= lambda1;
		  p_wv1_given_t1t2 += lambda1*wVnG.getCount(wv1, bigram)/c_t1t2;
		  
		  // Back off 1
		  double c_t1 = nGramsCount.getCounter(tag).totalCount();
		  double lambda2 = (remainder)*(1 - c_t1wvB/c_t1)*(1/(1+(nGramsCount.getCounter(tag).size()/c_t1)));
		  remainder -= lambda2;
		  p_wv1_given_t1t2 += lambda2*t1wv2_to_wv1.getCount(tag + " " + wvB.toString(), wv1.toString())/c_t1wvB;
		  
		  // Back off 2
		  double lambda3 = (remainder)*(1 - 1/c_t1)*(1/(1+(ncCount/(c_t1))));
		  remainder -= lambda3;
		  p_wv1_given_t1t2 += lambda3*wVnG.getCount(wv1, tag)/c_t1;

		  // Back off 3
		  double vf_total = wordCount.size()*featureCount;
		  double lambda4 = (remainder)*(1.0 - c_t1/vf_total)*0.5; // TODO This might not be correct
		  remainder -= lambda4;
		  p_wv1_given_t1t2 += lambda4*wordsToTags.getCount(wv1.word, tag)*tagsToFeatures.getCount(tag, wv1.feature)/(c_t1*c_t1);
		  
		  // Back off 4
		  p_wv1_given_t1t2 += (remainder)/(wordCount.size()*featureCount);
		  
		  return Math.log(p_wv1_given_t1t2);
	  }
	  
	  private double nonFirstWordBigram(WordVec wv1, WordVec wv2, String tag){
		  // Base Model
		  double remainder = 1.0;
		  String key = tag + " " + wv2.toString();
		  double c_t1wv2 = t1wv2_to_wv1.getCounter(key).totalCount();
		  double p_wv1_given_t1wv2 = 0.0;
		  double c_t1 = nGramsCount.getCounter(tag).totalCount();
		  double lambda1 = (1.0 - c_t1wv2/c_t1)*(1.0/(1.0+(nGramsCount.getCounter(tag).size()/c_t1)));
		  remainder -= lambda1;
		  p_wv1_given_t1wv2 += lambda1*t1wv2_to_wv1.getCount(key, wv1.toString())/c_t1wv2;
		  
		  // Back off 1
		  double lambda2 = (remainder)*(1.0 - 1.0/c_t1)*(1.0/(1.0+(ncCount/(c_t1))));
		  remainder -= lambda2;
		  p_wv1_given_t1wv2 += lambda2*wVnG.getCount(wv1, tag)/c_t1;

		  // Back off 2
		  double vf_total = wordCount.size()*featureCount;
		  double lambda3 = (remainder)*(1.0 - c_t1/vf_total)*0.5; // TODO this needs to be fixed
		  remainder -= lambda3;
		  p_wv1_given_t1wv2 += lambda3*wordsToTags.getCount(wv1.word, tag)*tagsToFeatures.getCount(tag, wv1.feature)/(c_t1*c_t1);
		  
		  // Back off 3
		  p_wv1_given_t1wv2 += (remainder)/(vf_total);
		  
		  return Math.log(p_wv1_given_t1wv2);
	  }
	  
	  private double nameClassBigram(String tag, List<String> previousTags, List<String> previousWords){
		  // Base Model expanded to NGram
		  double remainder = 1.0;
		  double p_t1_given_tnwn = 0.0;
		  String nGram = "";
		  double c_tn = 0.0;
		  
		  for(int n = nGramMax-1; n > 0; n--){
			  // Make key for tnwn
			  String key_tnwn = previousTags.get(0) + " " + previousWords.get(0);
			  for(int i = 1; i < n; i++){ key_tnwn += " " + previousTags.get(i) + " " + previousWords.get(i); }
			  
			  // Make ngram strings
			  List<String> nGramList = new ArrayList<String>(previousTags.subList(0, n));
			  String nTags = makeNGramString(nGramList);
			  nGram = tag + " " + nTags;
			  
			  // Calculate weight & base model
			  double c_tnwn = tnwn_to_t1.getCounter(key_tnwn).totalCount();
			  c_tn = nGramsCount.getCounter(nTags).totalCount();
			  double lambda = (remainder)*(1.0 - c_tnwn/c_tn)*(1/(1+(nGramsCount.getCounter(nTags).size()/c_tn)));
			  remainder -= lambda;
			  p_t1_given_tnwn += lambda*tnwn_to_t1.getCount(key_tnwn, tag)/c_tnwn;
		  }
		  
//		  double p_t1_given_t2w2 = 0.0;
//		  String key_t2w2 = previousTag + " " + previousWord;
//		  double c_t2w2 = t2w2_to_t1.getCounter(key_t2w2).totalCount();
//  		  String bigram = makeNGramString(Arrays.asList(tag, previousTag));
//		  double c_t2 = nGramsCount.getCounter(previousTag).totalCount();
//		  double lambda1 = (1.0 - c_t2w2/c_t2)*(1/(1+(nGramsCount.getCounter(previousTag).size()/c_t2)));
//		  remainder -= lambda1;
//		  p_t1_given_t2w2 += lambda1*t2w2_to_t1.getCount(key_t2w2,tag)/c_t2w2;

		  // Back off 1
		  double lambda2 = (remainder)*(1.0 - c_tn/totalCount)*(1/(1+(ncCount/totalCount)));
		  remainder -= lambda2;
		  p_t1_given_tnwn += lambda2*nGramsCount.getCounter(nGram).totalCount()/c_tn;
		  
		  // Back off 2
		  double c_t1 = nGramsCount.getCounter(tag).totalCount();
		  double lambda3 = (remainder)*(1.0 - 1.0/ncCount)*(0.5); // TODO this might need to be fixed
		  remainder -= lambda3;
		  p_t1_given_tnwn += lambda3*c_t1/totalCount;
		  
		  // Back off 3
		  p_t1_given_tnwn += (remainder)/ncCount;
		  
		   
		  return Math.log(p_t1_given_tnwn);
	  }
	  
	  private String makeNGramString(List<String> tags) {
		  String str = "";
		  for(String tag: tags){
			  str += tag + " ";
		  }
		  return str.trim();
	  }
	  
	  CounterMap<String, String> wordsToTags = new CounterMap<String, String>();
	  CounterMap<WordVec, String> wVnG = new CounterMap<WordVec, String>();

	  Counter<String> wordCount = new Counter<String>();
	  CounterMap<String, String> nGramsCount = new CounterMap<String, String>();
	  CounterMap<String, String> tnwn_to_t1 = new CounterMap<String, String>();
	  CounterMap<String, String> t1wv2_to_wv1 = new CounterMap<String, String>();
	  CounterMap<String, WordVec.Feature> tagsToFeatures = new CounterMap<String, WordVec.Feature>();
	  double totalCount = 0.0;
	  double ncCount = 0.0;
	  double featureCount = 14.0;
	  
	  public void train(List<LabeledLocalNGramContext> labeledLocalNGramContexts) {
		  int halfSize = Math.round(labeledLocalNGramContexts.size()/2);
		  
		  // Train vocabulary
		  // TODO generalize for nGrams
		  for(int i = 0; i < labeledLocalNGramContexts.size(); i++){
			  LabeledLocalNGramContext labeledLocalNGramContext = labeledLocalNGramContexts.get(i);
			  int position = labeledLocalNGramContext.getPosition();
			  String word = labeledLocalNGramContext.getCurrentWord();
			  String previousWord = labeledLocalNGramContext.getNthPreviousWord(0);	
			  List<String> previousWords = labeledLocalNGramContext.getNPreviousWords(nGramMax-1);
			  
			  String tag = labeledLocalNGramContext.getCurrentTag();
			  String previousTag = labeledLocalNGramContext.getNthPreviousTag(0);
			  List<String> previousTags = labeledLocalNGramContext.getNPreviousTags(nGramMax-1);
			  
			  boolean isWordFirstWord = (position == 0)? true: false;
			  boolean isPreviousWordFirstWord = (position == 1)? true: false;
			  
			  // Train for unknown with second half
			  if( i > halfSize){
				  if (!wordCount.containsKey(word)) word = UNKNOWN_WORD;
				  for(int j = 0; j < previousWords.size(); j++){
					  if (!wordCount.containsKey(previousWords.get(j))) previousWords.set(j, UNKNOWN_WORD);
				  }
				  if (!wordCount.containsKey(previousWord)) previousWord = UNKNOWN_WORD;
			  }
			  
			  // Create word vectors
			  WordVec wv1 = new WordVec(word, isWordFirstWord);
			  WordVec wv2 = new WordVec(previousWord, isPreviousWordFirstWord);
			  String bigram = makeNGramString(Arrays.asList(tag, previousTag));
			  
			  // Deal with +end+ word
			  if(tag != previousTags.get(0)){
				  // Make key for tnwn
				  String key_tnwn = previousTags.get(0) + " " + previousWords.get(0);
//				  for(int j = 1; j < n; n++){ key_tnwn += " " + previousTags.get(j) + " " + previousWords.get(j); }
				  
//				  t2w2_to_t1.incrementCount(previousTag + " " + END_WORD, tag, 1.0);
				  tnwn_to_t1.incrementCount(previousTags.get(0) + " " + END_WORD, tag, 1.0);
				  wordsToTags.incrementCount(END_WORD, previousTag, 1.0); // For Equation 3.6

				  WordVec wvE = new WordVec(END_WORD, false);
				  t1wv2_to_wv1.incrementCount(previousTag + " " + wv2.toString(), wvE.toString(), 1.0);
				  
				  WordVec wvB = new WordVec(BEGIN_WORD, false);
				  t1wv2_to_wv1.incrementCount(tag + " " + wvB.toString(), wv1.toString(), 1.0);
				  
			  }
			  			  
			  // Increment Counts
			  wVnG.incrementCount(wv1, tag, 1.0);
			  wVnG.incrementCount(wv1, bigram, 1.0);
			  
			  t1wv2_to_wv1.incrementCount(tag + " " + wv2.toString(), wv1.toString(), 1.0);
			  wordsToTags.incrementCount(word, tag, 1.0);
			  wordCount.incrementCount(word, 1.0);
			  nGramsCount.incrementCount(tag, word, 1.0);
			  tagsToFeatures.incrementCount(tag, wv1.feature, 1.0);

			  String nGram = "";
			  for(int n = nGramMax-1; n > 0; n--){
				  // Make key for tnwn
				  String key_tnwn = previousTags.get(0) + " " + previousWords.get(0);
				  for(int j = 1; j < n; j++){ key_tnwn += " " + previousTags.get(j) + " " + previousWords.get(j); }
				  
				  // Make ngram strings
				  List<String> nGramList = new ArrayList<String>(previousTags.subList(0, n));
				  String nTags = makeNGramString(nGramList);
				  nGram = tag + " " + nTags;
				  
				  // Increment counts
				  tnwn_to_t1.incrementCount(key_tnwn, tag, 1.0);
				  nGramsCount.incrementCount(nGram, word, 1.0);
			  }

			  // TODO account for counting in first position of nGram
			  if(position == 0){
				  word = previousWords.get(0);
				  wv1 = wv2;
				  tag = previousTag;
				  bigram = makeNGramString(Arrays.asList(tag, START_TAG));
				  wVnG.incrementCount(wv1, tag, 1.0);
				  wVnG.incrementCount(wv1, bigram, 1.0);
				  tnwn_to_t1.incrementCount(START_TAG + " " + START_WORD, tag, 1.0);
				  
				  wordsToTags.incrementCount(word, tag, 1.0);
				  wordCount.incrementCount(word, 1.0);
				  nGramsCount.incrementCount(tag, word, 1.0);
				  nGramsCount.incrementCount(bigram, word, 1.0);
				  tagsToFeatures.incrementCount(tag, wv1.feature, 1.0);
				  
				  wordsToTags.incrementCount(END_WORD, previousTag, 1.0); // For Equation 3.6
			  }		  			  
		  }
		  totalCount = wordCount.totalCount();
		  ncCount = tagsToFeatures.size();
	  }

	  public void validate(List<LabeledLocalNGramContext> localNGramContexts) {
	      // no tuning for this dummy model!
	  }
	  
	  public HMMTagScorer(int nGramMax, boolean restrictNGrams) {
		  setNGramMax(nGramMax);
		  this.restrictNGrams = restrictNGrams;
	  }
  }
  
  static class WordVec {
	  String word;
	  Feature feature;
	  
	  private enum Feature {
		  twoDigitNum,
		  fourDigitNum,
		  containsDigitAndAlpha,
		  containsDigitAndDash,
		  containsDigitAndSlash,
		  containsDigitAndComma,
		  containsDigitAndPeriod,
		  otherNum,
		  allCaps,
		  capPeriod,
		  firstWord,
		  initCap,
		  lowerCase,
		  other
	  }
	  
	  public boolean equals(Object o){
		  if (this == o) return true;
		  if (!(o instanceof WordVec)) return false;
		  
		  final WordVec wordVec = (WordVec) o;

		  if (word != null ? !word.equals(wordVec.word) : wordVec.word != null) return false;
		  if (feature != null ? !feature.equals(wordVec.feature) : wordVec.feature != null) return false;

		  return true;
	  }
	  
	  public WordVec(String word, boolean firstWord){
		  this.word = word;
		  this.feature = determineFeature(word, firstWord);
	  }
	  
	  private Feature determineFeature(String word, boolean firstWord){
		  Feature feature = Feature.other;
		  if(word.matches("\\d{2}")) feature = Feature.twoDigitNum;
		  else if(word.matches("\\d{4}")) feature = Feature.fourDigitNum;
		  else if(word.matches(".*[0-9].*")){
			  if(word.matches(".*[A-Za-z].*")) feature = Feature.containsDigitAndAlpha;
			  if(word.matches(".*[-].*")) feature = Feature.containsDigitAndDash;
			  if(word.matches(".*[/].*")) feature = Feature.containsDigitAndSlash;
			  if(word.matches(".*[,].*")) feature = Feature.containsDigitAndComma;
			  if(word.matches(".*[.].*")) feature = Feature.containsDigitAndPeriod;
			  else feature = Feature.otherNum;
		  }
		  else if(word.matches("[A-Z]+")) feature = Feature.allCaps;
		  else if(word.matches("[A-Z][a-z]+[.]")) feature = Feature.capPeriod;
		  else if(firstWord) feature = Feature.firstWord;
		  else if(word.matches("[A-Z][a-z]+")) feature = Feature.initCap;
		  else if(word.matches("[a-z]+")) feature = Feature.lowerCase;
		  else feature = Feature.other;
		  
		  return feature;
	  }
	  
	  public String toString(){
		  return word + " " + feature.toString();
	  }

	  public int hashCode() {
		  int result;
		  result = (word != null ? word.hashCode() : 0);
		  result = 29 * result + (feature.toString() != null ? feature.toString().hashCode() : 0);
		  return result;
	  }
  }
  
  private static String getTypeForTag(String tag){
	  switch(tag){
	  case "ORGANIZATION": return "ENAMEX";
	  case "PERSON": return "ENAMEX";
	  case "LOCATION": return "ENAMEX";
	  case "DATE": return "TIMEX";
	  case "TIME": return "TIMEX";
	  case "MONEY": return "NUMEX";
	  case "PERCENT": return "NUMEX";
	  default: return "UNKNOWN";
	  }
  }
  
  private static void labelTestDoc(NETagger neTagger, String corpus, String path, String outpath) throws IOException{
	  BufferedReader br = new BufferedReader(new FileReader(path));
	  BufferedWriter bw = new BufferedWriter(new FileWriter(outpath));

	  Pattern nePattern = Pattern.compile(NEPATTERN, Pattern.DOTALL);
	  Pattern sentencePattern = Pattern.compile("(<.+?>)|([^<>]+)", Pattern.DOTALL);
	  Pattern tagPattern = Pattern.compile("(<.+?>)", Pattern.DOTALL);
	  Pattern endWordPattern = Pattern.compile("([.,%]|'s)", Pattern.DOTALL);
	  
	  String line = null;  
	  String taggedLine = null;
      String lastTag = null;
      boolean writeStarted = false;
      List<int[]> indexList;

	  while ((line = br.readLine()) != null) {
		  indexList = new ArrayList<int[]>();
		  
		  Matcher split = sentencePattern.matcher(line);
		  taggedLine = line;
		  lastTag = START_TAG;
		  boolean needsEndTag = false;
		  while(split.find()){
			  
			  String subLine = split.group(0);
			  int startOffset = split.start();
			  
			  // Check if last line ends without a tag, else proceed as normal
			  if(!tagPattern.matcher(subLine).find()) {
				  
				  Matcher wordSplit = nePattern.matcher(subLine);
				  List<String> words = new ArrayList<String>();
				  while(wordSplit.find()){
					  indexList.add(new int[]{wordSplit.start() + startOffset, wordSplit.end() + startOffset});
					  words.add(wordSplit.group(0));
				  }
			      List<String> boundedWords = new BoundedList<String>(words, START_WORD, STOP_WORD);
			      List<String> tags = neTagger.tag(boundedWords);
			      List<Integer> insertIndex = new ArrayList<Integer>();
			      List<String> docTags = new ArrayList<String>();
			      
			      for(int i = 0; i < tags.size(); i++){
			    	  String tag = tags.get(i);
			    	  boolean lastIndex = (i == tags.size() - 1);
			    	  if(!tag.equals(lastTag)){
			    		  // First tag the end tag for the preceding word
			    		  if(lastTag != NAN_TAG && needsEndTag) {
			    			  needsEndTag = false;
				    		  String tagType = getTypeForTag(lastTag);
			    			  docTags.add("</"+tagType+">");
			    			  insertIndex.add(indexList.get(i-1)[1]);
			    		  }
			    		  // Then tag the start of the new tag
			    		  if(tag != NAN_TAG) {
			    			  needsEndTag = true;
				    		  String tagType = getTypeForTag(tag);
			    			  docTags.add("<" + tagType + " TYPE=\""+tag+"\">");
			    			  insertIndex.add(indexList.get(i)[0]);
			    		  }
			    		  
			    	  } 
			    	  
			    	  if(lastIndex && needsEndTag){
		    			  needsEndTag = false;
		    			  String tagType = getTypeForTag(tag);
		    			  docTags.add("</"+tagType+">");
		    			  insertIndex.add(indexList.get(i)[1]);
		    		  }
			    	  
			    	  lastTag = tag;
			      }
			      
			      // Iterate through this backwards so that the indices are valid
			      for(int i = docTags.size()-1; i >= 0; i--){
			    	  String word = docTags.get(i);
			    	  int index = insertIndex.get(i);
			    	  taggedLine = new StringBuilder(taggedLine).insert(index, word).toString();
			      }
			  }
		  }

		  // Write to file
		  if(writeStarted) bw.newLine();
		  else writeStarted = true;
		  bw.write(taggedLine);
	  } 

	  br.close();
	  bw.close();
	  
	  System.out.println("File written to: " + outpath);

  }
  

  private static List<TaggedSentence> readTaggedSentences(String corpus, String path) {
	  List<TaggedSentence> taggedSentences = new ArrayList<TaggedSentence>();

	  switch(corpus){
	  	case "muc6": {
	  		System.out.print("Muc 6 Corpus...");
	  		try {
				taggedSentences = readMUCData(path);
			} catch (IOException e) {
				e.printStackTrace();
			}

	  		break;
	  	}
	  	case "met1": {
	  		System.out.print("Met 1 Corpus...");
	  		try {
	  			// TODO Create MET Reader
	  			taggedSentences = readMUCData(path);
			} catch (IOException e) {
				e.printStackTrace();
			}

	  		break;
	  	}
	  	default: {
	  		System.out.print("Invalid corpus selection...");
	  		break;
	  	}
	  }
		
	  return taggedSentences;
  }
  
  private static List<TaggedSentence> readMUCFile(File file) throws IOException{
	  List<TaggedSentence> taggedSentences = new ArrayList<TaggedSentence>();
	  BufferedReader br = new BufferedReader(new FileReader(file));

	  String line = null;
	  String doc = null;
	  	  	  
	  while ((line = br.readLine()) != null) {
		  if(line.equals("<DOC>")){
			  doc = line;
		  } else {
			  doc += "\n" + line;
			  if(line.equals("</DOC>")){
				  MUCDocument document = new MUCDocument(doc);
				  taggedSentences.addAll(document.getTaggedSentences());
			  }
		  }
	  }

	  br.close();
	  
	  return taggedSentences;
  }
  
  private static List<TaggedSentence> readMUCData(String path) throws IOException {
	  List<TaggedSentence> taggedSentences = new ArrayList<TaggedSentence>();
	  	  
	  File dir = new File(path);
	  if(dir.isFile()){
		  taggedSentences = readMUCFile(dir);
	  } else if(dir.isDirectory()){
		  File[] dirList = dir.listFiles();
		  if(dirList != null){
			  for( File file : dirList){
				  taggedSentences.addAll(readMUCFile(file));
			  }
		  }
	  }
	  	  
	  return taggedSentences;
  }

  private static void evaluateTagger(NETagger neTagger, List<TaggedSentence> taggedSentences, Set<String> trainingVocabulary, boolean verbose) {
    double numTags = 0.0;
    double numTagsCorrect = 0.0;
    double numUnknownWords = 0.0;
    double numUnknownWordsCorrect = 0.0;
    double numNETags = 0.0;
    double numNETagsCorrect = 0.0;
    int numDecodingInversions = 0;
    for (TaggedSentence taggedSentence : taggedSentences) {
      List<String> words = taggedSentence.getWords();
      List<String> goldTags = taggedSentence.getTags();
      List<String> guessedTags = new BoundedList<String>(neTagger.tag(words), START_TAG, STOP_TAG);
      for (int position = 0; position < words.size(); position++) {
        String word = words.get(position);
        String goldTag = goldTags.get(position);
        String guessedTag = guessedTags.get(position);
        if (guessedTag.equals(goldTag))
          numTagsCorrect += 1.0;
        numTags += 1.0;
        if (!trainingVocabulary.contains(word)) {
          if (guessedTag.equals(goldTag))
            numUnknownWordsCorrect += 1.0;
          numUnknownWords += 1.0;
        }
        if(!goldTag.equals(NAN_TAG) && !goldTag.equals(START_TAG) && !goldTag.equals(STOP_TAG)){
        	if (guessedTag.equals(goldTag))
                numNETagsCorrect += 1.0;
        	numNETags += 1.0;
        }
      }
      double scoreOfGoldTagging = neTagger.scoreTagging(taggedSentence);
      double scoreOfGuessedTagging = neTagger.scoreTagging(new TaggedSentence(words, guessedTags));
      if (scoreOfGoldTagging > scoreOfGuessedTagging) {
        numDecodingInversions++;
        if (verbose) System.out.println("WARNING: Decoder suboptimality detected.  Gold tagging has higher score than guessed tagging.");
      }
//      System.out.println(alignedTaggings(words, goldTags, guessedTags, true));
//      if (verbose) System.out.println(alignedTaggings(words, goldTags, guessedTags, true));
    }
    System.out.printf("NE Accuracy: %.3f (Tag Accuracy: %.3f) (Unknown Accuracy: %.3f) Decoder Suboptimalities Detected: %d\n", numNETagsCorrect/numNETags, numTagsCorrect / numTags, numUnknownWordsCorrect / numUnknownWords, numDecodingInversions);
  }

  // pretty-print a pair of taggings for a sentence, possibly suppressing the tags which correctly match
  private static String alignedTaggings(List<String> words, List<String> goldTags, List<String> guessedTags, boolean suppressCorrectTags) {
    StringBuilder goldSB = new StringBuilder("Gold Tags: ");
    StringBuilder guessedSB = new StringBuilder("Guessed Tags: ");
    StringBuilder wordSB = new StringBuilder("Words: ");
    for (int position = 0; position < words.size(); position++) {
      equalizeLengths(wordSB, goldSB, guessedSB);
      String word = words.get(position);
      String gold = goldTags.get(position);
      String guessed = guessedTags.get(position);
      wordSB.append(word);
      if (position < words.size() - 1)
        wordSB.append(' ');
      boolean correct = (gold.equals(guessed));
      if (correct && suppressCorrectTags)
        continue;
      guessedSB.append(guessed);
      goldSB.append(gold);
    }
    return goldSB + "\n" + guessedSB + "\n" + wordSB;
  }

  private static void equalizeLengths(StringBuilder sb1, StringBuilder sb2, StringBuilder sb3) {
    int maxLength = sb1.length();
    maxLength = Math.max(maxLength, sb2.length());
    maxLength = Math.max(maxLength, sb3.length());
    ensureLength(sb1, maxLength);
    ensureLength(sb2, maxLength);
    ensureLength(sb3, maxLength);
  }

  private static void ensureLength(StringBuilder sb, int length) {
    while (sb.length() < length) {
      sb.append(' ');
    }
  }

  private static Set<String> extractVocabulary(List<TaggedSentence> taggedSentences) {
    Set<String> vocabulary = new HashSet<String>();
    for (TaggedSentence taggedSentence : taggedSentences) {
      List<String> words = taggedSentence.getWords();
      vocabulary.addAll(words);
    }
    return vocabulary;
  }

  private static class ChunkingCollector implements ObjectHandler<Chunking> {

	  private final List<Chunking> mChunkingList = new ArrayList<Chunking>();
	  public void handle(Chunking chunking) {
		  mChunkingList.add(chunking);
	  }
  }
  
  public static void main(String[] args) throws IOException {
    // Parse command line flags and arguments
    Map<String, String> argMap = CommandLineUtils.simpleCommandLineParser(args);

    // Set up default parameters and settings
    String basePath = ".";
    String testFile = "";
    String testKeyFile = "";
    boolean verbose = false;
    String corpus = "muc6";

    // The path to the assignment data
    if (argMap.containsKey("-path")) {
      basePath = argMap.get("-path");
    }
    System.out.println("Using base path: " + basePath);
    
    // Which corpus to use
    if (argMap.containsKey("-corpus")){
        corpus = argMap.get("-corpus").toLowerCase(); 
    }
    
    // Which test to use
    if (argMap.containsKey("-test")){
    	switch(corpus){
    	case "muc6":{
	        switch(argMap.get("-test").toLowerCase()) {
	        case "formal":
	        	testFile = "ne-co.formal.test.texts";
	        	testKeyFile = "formal-tst.NE.key.04oct95";
	        	break;
	        case "dryrun-single":
	        	testFile = "NE-CO.dryrun-single";
	        	testKeyFile = "dryrun-test.NE.key.02may95-single";
	        	break;
	        case "dryrun-uppercase":
		        testFile = "NE-CO.dryrun-edited-uppercase";
		        testKeyFile = "dryrun-test.NE.key.02may95-edited-uppercase";
		        break;
	        default:
		        testFile = "NE-CO.dryrun-edited";
		        testKeyFile = "dryrun-test.NE.key.02may95-edited";
		        break;
	        }
	        break;
	    }
    	case "twitter":{
    		switch(argMap.get("-test").toLowerCase()) {
	        case "formal":
	        	testFile = "ner-formal";
	        	testKeyFile = "ner-formal";
	        	break;
	        default:
		        testFile = "ner-dryrun";
		        testKeyFile = "ner-dryrun";
		        break;
	        }
	        break;
    	}
    	default:
    		break;
	    }
    }

    // Whether or not to print the individual errors.
    if (argMap.containsKey("-verbose")) {
      verbose = true;
    }
    
    String testFilePath = basePath + "texts/" + testFile + ".txt";
    String taggedFilePath = basePath + "texts/" + testFile + "-tagged.txt";
    String keyFilePath = basePath + "keys/" + testKeyFile + ".txt";
    String trainFilePath = basePath + "keys/dryrun-trng.NE-combined.key.v1.3.clean";

    // Read in data
    System.out.print("Loading training sentences...");
    List<TaggedSentence> trainTaggedSentences = readTaggedSentences(corpus, trainFilePath);
    Set<String> trainingVocabulary = extractVocabulary(trainTaggedSentences);
    System.out.println("done.");
    System.out.print("Loading tagged test sentences...");
    List<TaggedSentence> testTaggedSentences = readTaggedSentences(corpus, keyFilePath);
    System.out.println("done.");

    // Construct tagger components
    LocalNGramScorer localNGramScorer;
    TrellisDecoder<State> trellisDecoder;
    int nGramMax = 2;
    
    if (argMap.containsKey("-ngrams")){
    	nGramMax = Integer.parseInt(argMap.get("-ngrams"));
	}
    
	localNGramScorer = new HMMTagScorer(nGramMax, false);
	trellisDecoder = new ViterbiDecoder<State>();

    // Train tagger
    NETagger neTagger = new NETagger(localNGramScorer, trellisDecoder);
    neTagger.train(trainTaggedSentences);

    // Test tagger
    try {
		labelTestDoc(neTagger, corpus, testFilePath, taggedFilePath);
	} catch (IOException e) {
		e.printStackTrace();
	}
    evaluateTagger(neTagger, testTaggedSentences, trainingVocabulary, verbose);
        
    // Score using lingpipe's scorer
    File refFile = new File(keyFilePath);
    File responseFile = new File(taggedFilePath);
    ChunkingCollector refCollector = new ChunkingCollector();
    @SuppressWarnings("deprecation")
	Parser<ObjectHandler<Chunking>> parser = new  Muc6ChunkParser();
    parser.setHandler(refCollector);
    parser.parse(refFile);
    FileScorer scorer = new FileScorer(parser);
    scorer.score(refFile,responseFile);
    System.out.println(scorer.evaluation().toString());

  }
}