// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();

    const FString WordListPath = FPaths::ProjectContentDir() / TEXT("WordLists/HiddenWordList.txt");
    FFileHelper::LoadFileToStringArrayWithPredicate(Isograms, *WordListPath,[](const FString& Word){
        return 
            Word.Len() <= 8 && 
            Word.Len() >= 4 && 
            IsIsogram(Word);
    });

    SetupGame();
}

void UBullCowCartridge::OnInput(const FString& PlayerInput) // When the player hits enter
{
    
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else // Checking player guess
    {
        ProcessGuess(PlayerInput);
    }
    
}

void UBullCowCartridge::SetupGame()
{
    PrintLine(TEXT("Welcome to Bull Cows!"));

    HiddenWord = Isograms[FMath::RandRange(0, Isograms.Num() - 1)];
    Lives = HiddenWord.Len();
    bGameOver = false;

    //Welcoming the player
    PrintLine(TEXT("Guess the %i letter word"), HiddenWord.Len());
    PrintLine(TEXT("The HiddenWord is %s."), *HiddenWord);
    PrintLine(TEXT("You have %i lives."), Lives);
    PrintLine(TEXT("Type in your guess\nPress enter to continue..."));

}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("Press enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString& PlayerGuess)
{  
    if (PlayerGuess == HiddenWord)
    {
        PrintLine(TEXT("You have Won"));
        EndGame();
        return;
    }

    if (!IsIsogram(HiddenWord))
    {
        PrintLine(TEXT("No repeating letters, guess again"));
        return;
    }

    if (PlayerGuess.Len() != HiddenWord.Len())
    {         
        PrintLine(TEXT("The word is %i characters long."),HiddenWord.Len());       
        PrintLine(TEXT("Sorry, try guessing again."),Lives);
        return;
    }


    if (--Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left!"));
        PrintLine(TEXT("The word was: %s"),*HiddenWord);
        EndGame();  
        return;
    }

    PrintLine(TEXT("You've lost a life!"));
    PrintLine(TEXT("You have %i lives left"),Lives);
    PrintLine(TEXT("Guess again."));

    FBullCowCount Score = GetBullCows(PlayerGuess);
    PrintLine(TEXT("You have %i Bulls & %i Cows"), Score.Bulls, Score.Cows);

    //Show bulls and cows
}

bool UBullCowCartridge::IsIsogram(const FString& Word)
{
    for (int32 i = 0; i < Word.Len(); i++)
    {
        for (int32 j = i + 1; j < Word.Len(); j++)
        {
            if (Word[i] == Word[j]){
                return false;
            }
        }

    }
    //Loop, check each element on each element, except itself. Did it repeat?
    // If one is the same, return false;
    return true;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString& PlayerGuess) const
{
    //BullCount = 0;
    //CowCount = 0;
    FBullCowCount Count;
    for (int32 GuessIndex = 0; GuessIndex < PlayerGuess.Len(); GuessIndex++)
    {
        if (PlayerGuess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (PlayerGuess[GuessIndex] == HiddenWord[HiddenIndex]){
                Count.Cows++;    
                break;
            }
        }
        
    }
    return Count;
}

