#pragma once

// Å‰‚ÆÅŒã‚ÉŠÉ‹}
static float EaseInOutSine(float progress)
{
	return (float)-(std::cos(M_PI * progress) - 1) / 2;
}

// o‚ª‘‚­‚ÄÅŒã‚É‚ä‚Á‚­‚è
static float EaseOutQuad(float progress)
{
	return (float)1 - (1 - progress) * (1 - progress);
}

static float EaseOutCirc(float progress)
{
	return (float)sqrt(1 - pow(progress - 1, 2));
}