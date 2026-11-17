!  lore_real.h -- real-world facts, always marked "[Out of the story: ...]".
!  Shared by every game built on the engine.

[ LoreReal;
    if (TopicAny('douglas', 'adams', 'dna')) {
        print "[Out of the story: Douglas Adams (1952-2001) was an English writer and humorist, born in Cambridge. He created The Hitchhiker's Guide to the Galaxy as a BBC Radio 4 comedy that first aired in March 1978, then turned it into a series of novels, five of them his own, ending with Mostly Harmless in 1992. He also wrote the Dirk Gently novels, co-wrote Last Chance to See with the zoologist Mark Carwardine, worked on Doctor Who as a writer and script editor, and co-wrote Infocom's 1984 Hitchhiker's text adventure with Steve Meretzky. He died in California in May 2001, at the age of 49. This game is an unofficial fan tribute and has no connection with his estate.]^";
        rtrue;
    }
    if (TopicAny('hitchhiker', 'hitchhikers', 'series', 'trilogy', 'novels', 'books', 'bbc')
            || (TopicAny('radio') && ~~TopicAny('ending'))) {
        print "[Out of the story: The Hitchhiker's Guide to the Galaxy began as a BBC Radio 4 comedy in 1978. Douglas Adams turned it into five novels: The Hitchhiker's Guide to the Galaxy (1979), The Restaurant at the End of the Universe (1980), Life, the Universe and Everything (1982), So Long, and Thanks for All the Fish (1984) and Mostly Harmless (1992). There was also a BBC television series (1981), a text adventure from Infocom (1984) and a film (2005). The later books were adapted for radio in 2004-2005; that adaptation of the fifth book has a happier ending than the novel, and this game includes both. A sixth novel, And Another Thing..., by Eoin Colfer, appeared in 2009.]^";
        rtrue;
    }
    if (TopicAny('mostly', 'harmless', 'fifth', '1992', 'ending', 'endings')) {
        print "[Out of the story: Mostly Harmless (1992) is the fifth and last Hitchhiker's novel. In it Arthur has settled on the planet Lamuella as a sandwich maker. Ford Prefect discovers that the Guide has been taken over by the Vogons and mails Arthur an unfinished new edition, the Guide Mk II. Trillian leaves their daughter Random with Arthur, and Random takes the Guide and Ford's ship to a parallel Earth, where Tricia McMillan, Trillian's counterpart, is covering the landing of a spacecraft. In the book the Earth is destroyed, in every version, and the story ends there. The 2005 radio adaptation rescues everybody to the Restaurant at the End of the Universe. This game has both endings; try for the happy one.]^";
        rtrue;
    }
    if (TopicAny('infocom', 'meretzky', 'adventure', 'text', 'parser', 'interactive', 'fiction')) {
        print "[Out of the story: Infocom's 1984 The Hitchhiker's Guide to the Galaxy was a text adventure written by Douglas Adams and Steve Meretzky. It was famous for being very difficult and very funny about it. This game owes it a debt, and tries to be rather kinder about guessing what you meant.]^";
        rtrue;
    }
    if (TopicAny('dirk', 'gently')) {
        print "[Out of the story: Dirk Gently's Holistic Detective Agency (1987) and The Long Dark Tea-Time of the Soul (1988) are Douglas Adams's two novels about a detective who believes in the fundamental interconnectedness of all things.]^";
        rtrue;
    }
    if (TopicAny('tardis', 'pirate', 'shada') || (TopicAny('doctor') && TopicAny('who', 'whos', 'doctor'))) {
        print "[Out of the story: Douglas Adams wrote for the BBC's Doctor Who in the late 1970s, including The Pirate Planet and City of Death, and was the programme's script editor for a season.]^";
        rtrue;
    }
    if (TopicAny('carwardine', 'chance')) {
        print "[Out of the story: Last Chance to See (1990) is Douglas Adams and Mark Carwardine's account of searching the world for endangered animals. It is also very funny.]^";
        rtrue;
    }
    if (TopicAny('42', 'forty-two', 'meaning', 'ultimate', 'question') ) {
        print "In the story, 42 is the answer that the great computer Deep Thought gave to the Ultimate Question of Life, the Universe and Everything, after seven and a half million years of thought. Nobody has been quite sure what the question was. ";
        print "[Out of the story: Douglas Adams said it was simply a joke, and that he chose an ordinary, small number on purpose.]^";
        rtrue;
    }
    rfalse;
];
