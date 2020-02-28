import tweepy
import sys

def access_Twitter(handle):
	consumer_key="iD1MxOsmkhm3auyyA8KOKeNiF"
	consumer_secret="rc7vmkFI7TEb0Jtd5deWOgwhSHOOvlPYw1CbPMhH4GLlQjQrzV"
	access_key="1009583856-D5nYFeOMC9mgA8yCqYdDdx0OzC3n80FhH8DmBDy"
	access_secret="pKzYDRPOLrufQZujbL4ckpFtD63oPi0pzpDS1jsd3lavK"

	api=tweepy.OAuthHandler(consumer_key, consumer_secret)
	api.set_access_token(access_key, access_secret)

	api_access=tweepy.API(api)

	timeline=[]

	try:
		timeline=api_access.user_timeline(screen_name=handle, count=200, tweet_mode="extended")
	except tweepy.TweepError as error:
		print("An error has occurred: ", error)
		return
	filename=handle+'.txt'
	file=open(filename, 'w')
	for i in range (0, len(timeline)):
		file.write(JustAscii(timeline[i].full_text).encode('ascii').replace('\n', " "))
		file.write('\n')
	file.close()
def JustAscii(s): 
	return "".join(i for i in s if ord(i)<128)

access_Twitter(sys.argv[1])
